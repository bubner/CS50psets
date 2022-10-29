import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    total = 0

    # Get money the user owns
    money = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    total += money[0]["cash"]

    # Grab all owned stocks from the user
    ownedstocks = db.execute("""SELECT symbol, SUM(quantity) AS quantity, time
                                FROM transactions WHERE user_id = ?
                                GROUP BY symbol HAVING SUM(quantity) > 0
                                ORDER BY symbol ASC""", session["user_id"])

    # Assign API values to owned stocks
    for stock in ownedstocks:
        api = lookup(stock["symbol"])
        stock["name"] = api["name"]
        stock["price"] = usd(api["price"])
        stock["total"] = usd(stock["quantity"] * api["price"])
        total += api["price"]

    return render_template("index.html", stocks=ownedstocks, money=usd(money[0]["cash"]), total=usd(total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not validate(shares, symbol):
            return apology("Invalid or missing information was entered")

        if not (query := lookup(symbol)):
            return apology("Couldn't find symbol")

        shares = int(shares)

        # Get financial data
        rows = db.execute("SELECT * FROM users WHERE id = ?;", session["user_id"])
        cash = rows[0]["cash"]
        total = query["price"] * shares
        if cash < total:
            return apology("Not enough money")

        # Log and complete transaction
        db.execute("INSERT INTO transactions(user_id, symbol, quantity, price, time) VALUES(?, ?, ?, ?, ?);",
                   session["user_id"], symbol, shares, query["price"], datetime.now().strftime("%Y-%m-%d %H:%M:%S"))

        # Update amount of money
        db.execute("UPDATE users SET cash = ? WHERE id = ?;",
                   (cash - total), session["user_id"])

        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    t = db.execute("SELECT * FROM transactions WHERE user_id = ?;", session["user_id"])
    return render_template("history.html", transactions=t)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        query = lookup(request.form.get("symbol"))
        if not query:
            return apology("Invalid symbol")

        return render_template("quote.html", query=query)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Check and make sure the form was filled out
        if not username:
            return apology("Missing username!")

        if not password:
            return apology("Missing password!")

        if not confirmation or password != confirmation:
            return apology("Password mismatch!")

        # Ensure this user doesn't already exist
        matchingusers = db.execute("SELECT * FROM users WHERE username = ?;", username)
        if len(matchingusers) != 0:
            return apology(f"Username '{username}' already exists.")

        # Check the password to ensure it matches standards
        if not validatepassword(password):
            return apology("Password needs 8 characters, 1 number and 1 character minimum!")

        # We have now confirmed this user is safe to put in our database
        id = db.execute("INSERT INTO users (username, hash) VALUES (?, ?);",
                        username, generate_password_hash(password))

        # Remember user
        session["user_id"] = id

        return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    owned_symbols = db.execute("""SELECT symbol, sum(quantity) as sum_of_shares
                                FROM transactions
                                WHERE user_id = ?
                                GROUP BY user_id, symbol
                                HAVING sum_of_shares > 0;""", session["user_id"])

    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not validate(shares, symbol):
            return apology("Invalid or missing information was entered")
        shares = int(shares)

        symbols_dict = {d['symbol']: d['sum_of_shares'] for d in owned_symbols}
        if symbols_dict[symbol] < shares:
            return apology("Too many shares")

        query = lookup(symbol)

        # Get user info
        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])

        # Log and complete transaction
        db.execute("INSERT INTO transactions(user_id, symbol, quantity, price, time) VALUES(?, ?, ?, ?, ?);",
                   session["user_id"], symbol, -shares, query["price"], datetime.now().strftime("%Y-%m-%d %H:%M:%S"))

        # Update amount of money
        db.execute("UPDATE users SET cash = ? WHERE id = ?;",
                   (rows[0]['cash'] + (query['price'] * int(shares))), session["user_id"])

        return redirect("/")
    else:
        return render_template("sell.html", symbols=owned_symbols)


# Add money to an account
@app.route("/addcash", methods=["GET", "POST"])
@login_required
def addmoney():
    if request.method == "POST":
        # Get user info
        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])

        # Check entry
        if not (addmoney := request.form.get("money")):
            return apology("Invalid entry")

        try:
            addmoney = float(addmoney)
            # Prevent negative money
            if addmoney + rows[0]["cash"] < 0:
                raise ValueError
        except ValueError:
            return apology("Malformed entry")

        # Update money
        db.execute("UPDATE users SET cash = ? WHERE id = ?",
                   (rows[0]["cash"] + addmoney), session["user_id"])

        return redirect("/")
    else:
        return render_template("addcash.html")


# Reset password route
@app.route("/forgot", methods=["GET", "POST"])
def resetpass():
    if request.method == "POST":
        # Check entries
        targetuser = request.form.get("targetuser")
        if not (newpass := request.form.get("newpass")) or not (targetuser := request.form.get("targetuser")):
            return apology("Invalid information")

        # Get users from the database
        databaseusers = db.execute("SELECT * FROM users WHERE username = ?", targetuser)
        print(targetuser, databaseusers)
        if targetuser not in databaseusers[0]["username"]:
            return apology("User does not exist")

        if not validatepassword(newpass):
            return apology("Entered password is too weak")

        # If username exists, change their password to the supplied one
        db.execute("UPDATE users SET hash = ? WHERE username = ?", generate_password_hash(newpass), targetuser)

        return redirect("/")
    else:
        return render_template("forgot.html")


# Function to check a password for 8 chars + number + character
def validatepassword(password):
    return len(password) >= 8 and any(char.isdigit() for char in password) and any(not char.isalnum() for char in password)


# Function that returns the validity of an entered symbol and share
def validate(shares, symbol):
    try:
        shares = int(shares)
        if not symbol or not shares or shares < 0:
            raise ValueError
    except ValueError:
        return False
    else:
        return True