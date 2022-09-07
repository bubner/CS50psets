
def main():
    card = ""
    # Validate input
    while not card:
        card = input('Please enter a credit card number (numbers only, no spaces): ')

    cardarr = []
    for num in card:
        try:
            cardarr.append(int(num))
        except ValueError:
            # If there aren't clean int-castable numbers, report invalid
            print("INVALID")
            exit(1)

    # Extract each second digit starting from the back
    std = alt = ""
    # Test if the length is odd and search +2 if it is, as the loop won't capture them normally
    for digit in range(-2, -len(cardarr) if len(cardarr) % 2 != 0 else -len(cardarr) - 2, -2):
        # Add these number values together, while adding the numbers not multiplied together to another value
        # Uses string for concatenation not addition of these numbers, to only add the actual digits
        std += str(cardarr[digit] * 2)
        alt += str(cardarr[digit + 1])

    # Tests if the array is odd and extract the first number, as the for loop won't get it if it is odd size due to the arguments we supplied
    if len(cardarr) % 2 != 0:
        alt += str(cardarr[0])

    # Add each digit together in std and alt
    stdout = altout = 0
    for digit in std:
        stdout += int(digit)
    for digit in alt:
        altout += int(digit)

    # stdout + altout is our final number to check

    # Testing phase to deduct what card it is
    if (stdout + altout) % 10 == 0:
        if (len(cardarr) == 15) and (str(cardarr[0]) + str(cardarr[1]) == "37"):
            print("AMEX")
        elif (len(cardarr) == 13 or len(cardarr) == 16) and str(cardarr[0]) + str(cardarr[1]) in ["51", "52", "53", "54", "55"]:
            print("MASTERCARD")
        elif (len(cardarr) == 13 or len(cardarr) == 16) and cardarr[0] == 4:
            print("VISA")
        else:
            print("LUHN'S ALGORITHM PASS")
            print("INVALID")
    else:
        print("INVALID")


if __name__ == "__main__":
    main()