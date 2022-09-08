
def main():
    # Set an empty string for the name
    name = ""
    # Loop until we get an input that isn't blank
    while not name:
        name = input("What is your name?\n")
    # Print it back to the user
    print(f"hello, {name}")


if __name__ == "__main__":
    main()