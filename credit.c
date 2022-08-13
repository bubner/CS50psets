#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

// Takes a credit card input and runs it against the Luhn's Algorithm, and input whether it is valid or invalid.
// Once completed the program will check against the parameters to determine which type of card it is.

/*
    CREDIT CARD PARAMETERS
    American Express == 15 digits, starts with 34 or 37
    MasterCard == 16 digits, starts with range 51-55
    Visa == 13 or 16 digits, starts with 4

    LUHNS ALGORITHM
    Multiply every other digit by 2, starting with the number’s second-to-last digit, and then add those products’ digits together.
    Add the sum to the sum of the digits that weren’t multiplied by 2.
    If the total’s last digit is 0 (or, put more formally, if the total modulo 10 is congruent to 0), the number is valid!
*/

// Trims number down and records how many times it iterated, effectively giving a number count
int countDigits(long input)
{
    int runs = 0;
    while (input != 0)
    {
        input /= 10;
        runs++;
    }
    return runs;
}

// Extracts all but requested digits, and returns what is left, giving the starting (left-most) digits
int startingDigits(long input, int digits, int leave)
{
    for (int i = 0; i < digits - leave; i++)
    {
        input /= 10;
    }
    return input;
}

// Checks whether the final digit is a zero and returns true if it is
bool checkLastDig(int input)
{
    return (input % 10 == 0) ? true
           :
           false;
}

int main(void)
{
    // Declare parameters here for card digit starters
    int amex[] = {34, 37};
    int mastercard[] = {51, 52, 53, 54, 55};

    // Get the user input for a credit card number, which should reject any non-long data type number
    long card = get_long("Please enter a credit card number (numbers only, no spaces): ");
    // Make an immutable state of the card so we don't lose it through mathematical calculations
    const long card_raw = card;
    // Make a buffer loop function to extract the number of digits in the card number
    int digits = countDigits(card);
    int carddigits = digits;
    // Checks if the number of digits is even (cannot be divided nicely by 2)
    if (digits % 2 != 0)
    {
        digits += 1;
        // If so, we make it even again (by adding +1 to the digit count)
        // The actual number of digits are saved before writing, to allow the checksum to work
        // The reason we do this is so the arrays can be split nicely with x/2, and so we don't have any memory issues
    }

    // Make a new loop for the purpose of extracting each digit and append them to an array
    int nums[digits];
    int numbuffer;
    for (int i = 0; i < digits; i++)
    {
        numbuffer = card % 10;
        nums[i] = numbuffer;
        card /= 10;
    }

    int std[digits / 2];
    // Loop over all the digits in the array and extract the alternating ones
    // Using digits / 2 in all half operations as we don't need an array that is bigger (we are only working with 50% of the numbers)
    // Ensure to use the digits += 1 variable so the arrays are initalised nicely for splitting
    for (int i = 0, x = 0; i < digits; i += 2, x++)
    {
        std[x] = nums[i];
    }

    // Remove final number from array (we have already original length saved to memory and an immutable variable holding the original)
    for (int i = 0; i < digits; i++)
    {
        nums[i] = nums[i + 1];
    }

    // Loop over the remaining digits and extract the alternating ones (for checksum calculation)
    int alt[digits / 2];
    for (int i = 0, x = 0; i < digits; i += 2, x++)
    {
        alt[x] = nums[i];
    }

    // Multiply each digit by 2 and allocate them to their specific array
    int x2[digits / 2];
    int x2n[digits / 2];
    int output = 0;
    for (int i = 0; i < digits / 2; i++)
    {
        // Grabs second digit
        x2[i] = (alt[i] * 2) % 10;
        // Grabs first (tens) digit
        x2n[i] = (alt[i] * 2) / 10;
        // Append adds the number places to the output -- we will never surpass three digits (9*2=18), so we do not need to account for that
        // We also grab the std array numbers (not multiplied by 2), and append all the numbers together into a new output (we don't have to split place value as they are 1 digit)
        // This means we can get our output quickly and save some time as well
        output += x2[i] + x2n[i] + std[i];
    }

    // A quick if statement function to test if the last digit is a zero, and we can return a boolean to dictate whether the checksum failed or was successful
    // Ensure to use the actual number of card digits
    if ((checkLastDig(output) == true) && (carddigits >= 13))
    {
        // Once we have confirmed that the card is indeed true via the math, we can run some quick checks on the structure to determine which kind of card it is
        // Tests the first digit if it is a 4 and also is 16 digits long, then returns Visa if it is
        if ((startingDigits(card_raw, carddigits, 1) == 4) && (carddigits == 13 || carddigits == 16))
        {
            printf("VISA\n");
            exit(0);
        }
        else
        {
            // Runs a loop over the array of Amex and Mastercard numbers provided at the top of the script, while being sure to break the loop if a match is found
            for (int i = 0; i < sizeof mastercard + sizeof amex; i++)
            {
                if (startingDigits(card_raw, carddigits, 2) == amex[i] && carddigits == 15)
                {
                    printf("AMEX\n");
                    exit(0);
                }
                else if (startingDigits(card_raw, carddigits, 2) == mastercard[i] && carddigits == 16)
                {
                    printf("MASTERCARD\n");
                    exit(0);
                }
            }
            // If the program is not exited, we know the card is invalid even when the math checks out
            // Be sure to mention that the Luhn's Algorithm was successful -- we only check for three types of cards
            printf("LUHN'S ALGORITHM PASS\n");
            printf("INVALID\n");
        }
    }
    else
    {
        // Skip testing the array and print invalid if the card isnt over 13 digits or doesn't pass the checksum
        printf("INVALID\n");
    }
}
