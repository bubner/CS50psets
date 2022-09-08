#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // Checks if argc is equal to 2, and spits an error out if it is not. argc[0] is the file name so we need to check for 2, not 1.
    if (argc != 2)
    {
        printf("Usage: ./substitution <key>\n");
        return 1;
    }

    // Uses 2D array argv[1] so we don't get the arguments of the compiler and instead the command line arguments
    const int keylen = strlen(argv[1]);

    // Checks if the given string length is 26 characters, and spits an error if it is not
    if (keylen != 26)
    {
        printf("Key must contain 26 letters. Input was: %i letters.\n", keylen);
        return 1;
    }

    // Declare a new array and set it to null, this will be used to declare which letters we've seen
    int seen[26] = {0};
    for (int i = 0; i < keylen; i++)
    {
        if (isalpha(argv[1][i]))
        {
            if (isupper(argv[1][i]))
            {
                // ASCII value 65 = upper A
                // Appends the frequency of each letter in the string
                // This should be equal to one
                seen[argv[1][i] - 65]++;
            }
            else
            {
                // ASCII value 97 = lowercase a
                // We do not need to check if it is lowercase if we already know it is not upper + a letter of the alphabet
                seen[argv[1][i] - 97]++;
            }
        }
        else
        {
            printf("Key must only contain alphabetical characters. Accepted range: a-z A-Z\n");
            return 1;
        }
    }
    
    // Finally, check if each letter has been seen at least once
    for (int i = 0; i < keylen; i++)
    {
        if (seen[i] != 1)
        {
            printf("Key must only contain one of each alphabetical character. Duplicate: %c\n", argv[1][i]);
            return 1;
        }
    }

    // Confirmed our key is good to go
    string text = get_string("plaintext: ");
    for (int i = 0, textlen = strlen(text); i < textlen; i++)
    {
        // Passes any non alphabetic character through, leaving it alone
        if (!isalpha(text[i]))
        {
            continue;
        }

        // Goes through the string and replaces characters that are in the cipher mentioned
        if (isupper(text[i]))
        {
            // ASCII 65 for capital letters
            // Uses toupper function to preserve upper case
            text[i] = argv[1][text[i] - 65];
            text[i] = toupper(text[i]);
        }
        else
        {
            // ASCII 97 for lowercase letters
            // Instead uses tolower to preserve lower case
            text[i] = argv[1][text[i] - 97];
            text[i] = tolower(text[i]);
        }
    }

    // All done, print cipher to user
    printf("ciphertext: %s\n", text);
    // End the program with exit code 0, success
    return 0;
}