-- Keep a log of any SQL queries you execute as you solve the mystery.

/*
We have this information:
    Event occurred on date July 28
    Event took place on Humphrey Street
and we are looking for:
    Suspect
    Location
    Accomplice
*/

-- Firstly, we need to query the crime scene reports table on July 28th, Humphrey Street
SELECT description FROM crime_scene_reports
WHERE month = 7
AND day = 28
AND street = "Humphrey Street";

-- NEW INFO: Theft of duck took place at 10:15am, Humphrey Street bakery

-- We need to query the bakery security log at the hour of the event
SELECT * FROM bakery_security_logs
WHERE month = 7
AND day = 28
AND hour = 10;
/*
+-----+------+-------+-----+------+--------+----------+---------------+
| id  | year | month | day | hour | minute | activity | license_plate |
+-----+------+-------+-----+------+--------+----------+---------------+
| 258 | 2021 | 7     | 28  | 10   | 8      | entrance | R3G7486       |
| 259 | 2021 | 7     | 28  | 10   | 14     | entrance | 13FNH73       |
| 260 | 2021 | 7     | 28  | 10   | 16     | exit     | 5P2BI95       |
| 261 | 2021 | 7     | 28  | 10   | 18     | exit     | 94KL13X       |
| 262 | 2021 | 7     | 28  | 10   | 18     | exit     | 6P58WS2       |
| 263 | 2021 | 7     | 28  | 10   | 19     | exit     | 4328GD8       |
| 264 | 2021 | 7     | 28  | 10   | 20     | exit     | G412CB7       |
| 265 | 2021 | 7     | 28  | 10   | 21     | exit     | L93JTIZ       |
| 266 | 2021 | 7     | 28  | 10   | 23     | exit     | 322W7JE       |
| 267 | 2021 | 7     | 28  | 10   | 23     | exit     | 0NTHK55       |
| 268 | 2021 | 7     | 28  | 10   | 35     | exit     | 1106N58       |
| 269 | 2021 | 7     | 28  | 10   | 42     | entrance | NRYN856       |
| 270 | 2021 | 7     | 28  | 10   | 44     | entrance | WD5M8I6       |
| 271 | 2021 | 7     | 28  | 10   | 55     | entrance | V47T75I       |
+-----+------+-------+-----+------+--------+----------+---------------+
*/

-- SUSPECTS: Vehicles, 5P2BI95, 94KL13X, 6P58WS2, 4328GD8, G412CB7, L93JTIZ, 322W7JE, 0NTHK55

-- We now need to check interviews of all people on the day, to gather more information
SELECT name, transcript from interviews
WHERE month = 7
AND day = 28;

-- NEW INFO: Within 10 minutes of the theft, an exit from the bakery was reported. This confirms one of the vehicles above as having the thief.
-- NEW INFO: Flight ticket was purchased to fly earliest flight out of Fiftyville tomorrow morning
-- NEW INFO: Lengett Street bakery ATM was used to withdraw money

-- We now need to analyse the logs from the Leggett Street bakery on the day
SELECT * FROM atm_transactions
WHERE month = 7
AND day = 28
AND atm_location = "Leggett Street";

-- There are a few transactions, but we need names. To do with we will join their bank account numbers to their name using the JOIN keyword
SELECT name
FROM people
JOIN bank_accounts
    ON people.id = bank_accounts.person_id
JOIN atm_transactions
    ON bank_accounts.account_number = atm_transactions.account_number
WHERE atm_transactions.month = 7
AND atm_transactions.day = 28
AND atm_transactions.atm_location = "Leggett Street"
AND atm_transactions.transaction_type = "withdraw";

/*
+---------+
|  name   |
+---------+
| Bruce   |
| Diana   |
| Brooke  |
| Kenny   |
| Iman    |
| Luca    |
| Taylor  |
| Benista |
+---------+
*/

-- NEW SUSPECTS: All these people who are on the list of transactions

-- Now, we need to investigate the other info we have receieved. A flight the next day from Fiftyville to an unknown city had been arranged. We are to find the flights on the 29th.
SELECT *
FROM airports
WHERE city = "Fiftyville";

/*
+----+--------------+-----------------------------+------------+
| id | abbreviation |          full_name          |    city    |
+----+--------------+-----------------------------+------------+
| 8  | CSF          | Fiftyville Regional Airport | Fiftyville |
+----+--------------+-----------------------------+------------+
*/

-- It is destined that the origin of the flight would originate from Fiftyville, so we are to take the abbreviation of this airport and compare it to the flight destinations on the 29th from Fiftyville
SELECT city, flights.hour, flights.minute
FROM airports
JOIN flights
    ON airports.id = flights.destination_airport_id
WHERE flights.origin_airport_id = (
    SELECT id FROM airports WHERE abbreviation = "CSF"
)
AND flights.month = 7
AND flights.day = 29
-- We know that our suspect took the earliest flight, so we will focus on that one. With this query, we should select the very first one.
ORDER BY flights.hour, flights.minute DESC
LIMIT 1;

/*
+---------------+------+--------+
|     city      | hour | minute |
+---------------+------+--------+
| New York City | 8    | 20     |
+---------------+------+--------+
*/

-- NEW INFO: New York City at 8:20 AM, earliest flight from Fiftyville to NYC.
-- We now must analyse who was on this flight.
SELECT *
FROM people
JOIN passengers
    ON people.passport_number = passengers.passport_number
JOIN flights
    ON flights.id = passengers.flight_id
WHERE flights.month = 7
AND flights.day = 29
AND flights.hour = 8
AND flights.minute = 20;

/*
+--------+--------+----------------+-----------------+---------------+-----------+-----------------+------+----+-------------------+------------------------+------+-------+-----+------+--------+
|   id   |  name  |  phone_number  | passport_number | license_plate | flight_id | passport_number | seat | id | origin_airport_id | destination_airport_id | year | month | day | hour | minute |
+--------+--------+----------------+-----------------+---------------+-----------+-----------------+------+----+-------------------+------------------------+------+-------+-----+------+--------+
| 953679 | Doris  | (066) 555-9701 | 7214083635      | M51FA04       | 36        | 7214083635      | 2A   | 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     |
| 398010 | Sofia  | (130) 555-0289 | 1695452385      | G412CB7       | 36        | 1695452385      | 3B   | 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     |
| 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       | 36        | 5773159633      | 4A   | 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     |
| 651714 | Edward | (328) 555-1152 | 1540955065      | 130LD9Z       | 36        | 1540955065      | 5C   | 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     |
| 560886 | Kelsey | (499) 555-9472 | 8294398571      | 0NTHK55       | 36        | 8294398571      | 6C   | 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     |
| 449774 | Taylor | (286) 555-6063 | 1988161715      | 1106N58       | 36        | 1988161715      | 6D   | 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     |
| 395717 | Kenny  | (826) 555-1652 | 9878712108      | 30G67EN       | 36        | 9878712108      | 7A   | 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     |
| 467400 | Luca   | (389) 555-5198 | 8496433585      | 4328GD8       | 36        | 8496433585      | 7B   | 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     |
+--------+--------+----------------+-----------------+---------------+-----------+-----------------+------+----+-------------------+------------------------+------+-------+-----+------+--------+
*/

-- Each of these people can be suspects, so we will check if they match any of the queries done before
SELECT name
FROM people
WHERE name IN (
    SELECT name
    FROM people
    JOIN bank_accounts
        ON people.id = bank_accounts.person_id
    JOIN atm_transactions
        ON bank_accounts.account_number = atm_transactions.account_number
    WHERE atm_transactions.month = 7
    AND atm_transactions.day = 28
    AND atm_transactions.atm_location = "Leggett Street"
    AND atm_transactions.transaction_type = "withdraw"

    INTERSECT

    SELECT name
    FROM people
    JOIN passengers
        ON people.passport_number = passengers.passport_number
    JOIN flights
        ON flights.id = passengers.flight_id
    WHERE flights.month = 7
    AND flights.day = 29
    AND flights.hour = 8
    AND flights.minute = 20
);

/*
+--------+
|  name  |
+--------+
| Kenny  |
| Taylor |
| Luca   |
| Bruce  |
+--------+
*/

-- These four people are now the primary suspects. However, this is not enough. We must check the phone calls made on the 28th for information, where the call reciever matches these names
SELECT *
FROM people
JOIN phone_calls
    ON people.phone_number = phone_calls.caller
WHERE name IN (
    SELECT name
    FROM people
    JOIN bank_accounts
        ON people.id = bank_accounts.person_id
    JOIN atm_transactions
        ON bank_accounts.account_number = atm_transactions.account_number
    WHERE atm_transactions.month = 7
    AND atm_transactions.day = 28
    AND atm_transactions.atm_location = "Leggett Street"
    AND atm_transactions.transaction_type = "withdraw"

    INTERSECT

    SELECT name
    FROM people
    JOIN passengers
        ON people.passport_number = passengers.passport_number
    JOIN flights
        ON flights.id = passengers.flight_id
    WHERE flights.month = 7
    AND flights.day = 29
    AND flights.hour = 8
    AND flights.minute = 20
)
-- We are also not interested in any dates other than the 28th
AND phone_calls.month = 7
AND phone_calls.day = 28;

/*
+--------+--------+----------------+-----------------+---------------+-----+----------------+----------------+------+-------+-----+----------+
|   id   |  name  |  phone_number  | passport_number | license_plate | id  |     caller     |    receiver    | year | month | day | duration |
+--------+--------+----------------+-----------------+---------------+-----+----------------+----------------+------+-------+-----+----------+
| 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       | 233 | (367) 555-5533 | (375) 555-8161 | 2021 | 7     | 28  | 45       |
| 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       | 236 | (367) 555-5533 | (344) 555-9601 | 2021 | 7     | 28  | 120      |
| 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       | 245 | (367) 555-5533 | (022) 555-4052 | 2021 | 7     | 28  | 241      |
| 449774 | Taylor | (286) 555-6063 | 1988161715      | 1106N58       | 254 | (286) 555-6063 | (676) 555-6554 | 2021 | 7     | 28  | 43       |
| 395717 | Kenny  | (826) 555-1652 | 9878712108      | 30G67EN       | 279 | (826) 555-1652 | (066) 555-9701 | 2021 | 7     | 28  | 55       |
| 449774 | Taylor | (286) 555-6063 | 1988161715      | 1106N58       | 284 | (286) 555-6063 | (310) 555-8568 | 2021 | 7     | 28  | 235      |
| 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       | 285 | (367) 555-5533 | (704) 555-5790 | 2021 | 7     | 28  | 75       |
+--------+--------+----------------+-----------------+---------------+-----+----------------+----------------+------+-------+-----+----------+
*/

-- We see Bruce is very promiment in these records. This increases the level of suspicion we have on him. Let's try to find the receiver of Bruce's call.
-- We know Bruce's number is (367) 555-5533
SELECT name, phone_calls.duration
FROM people
JOIN phone_calls
    ON people.phone_number = phone_calls.receiver
AND phone_calls.month = 7
AND phone_calls.day = 28
AND phone_calls.caller = "(367) 555-5533"
ORDER BY phone_calls.duration ASC;

/*
+---------+----------+
|  name   | duration |
+---------+----------+
| Robin   | 45       |
| Carl    | 75       |
| Deborah | 120      |
| Gregory | 241      |
+---------+----------+
    We know that the phone call lasted less than a minute, meaning we can make a conclusion
    that Robin is the accomplice if Bruce checks out to be the thief.
*/

-- Now, we need to find who's cars were there at the time during the theft at the bakery
-- We know that the exiting vehicle was 10 minutes from the theft
SELECT name, bakery_security_logs.hour, bakery_security_logs.minute, people.license_plate
FROM people
JOIN bakery_security_logs
    ON people.license_plate = bakery_security_logs.license_plate
WHERE bakery_security_logs.year = 2021
AND bakery_security_logs.month = 7
AND bakery_security_logs.day = 28
AND bakery_security_logs.hour = 10
AND bakery_security_logs.activity = 'exit'
ORDER BY bakery_security_logs.minute;

/*
+---------+------+--------+---------------+
|  name   | hour | minute | license_plate |
+---------+------+--------+---------------+
| Vanessa | 10   | 16     | 5P2BI95       |
| Bruce   | 10   | 18     | 94KL13X       |
| Barry   | 10   | 18     | 6P58WS2       |
| Luca    | 10   | 19     | 4328GD8       |
| Sofia   | 10   | 20     | G412CB7       |
| Iman    | 10   | 21     | L93JTIZ       |
| Diana   | 10   | 23     | 322W7JE       |
| Kelsey  | 10   | 23     | 0NTHK55       |
| Taylor  | 10   | 35     | 1106N58       |
+---------+------+--------+---------------+
*/

/*
    - CONCLUSION -
    We see Bruce's car is there at 10:18. This is a strong indicator this was him. We also see the other suspects
    from the calls (Taylor, Kenny) are not within the 10 minute timeframe.
    Also, the licence plate matches a suspect listed earlier, so we can make a logical conclusion.
    We can conclude Bruce is the thief, and he had taken the NYC flight.
    As for the accomplice, we can now confirm it was Robin, as the call lasted less than 1 minute.
*/

/* ===================
    Thief: Bruce
    Flight: NYC
    Accomplice: Robin
   =================== */
