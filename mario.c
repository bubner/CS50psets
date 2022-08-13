// Takes user input and displays hashes to make a mario-like tower throughout the range of numbers 1-8. Any other characters inputted will be rejected.

#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    // Loops in while true loop with conditional breaking to make sure the user has entered a proper answer (above 1, below 8, integer)
    while (true)
    {
        // get_int already protects against strings or other forms of variable
        height = get_int("Height in numbers: ");
        if (height > 0 && height <= 8)
        {
            // Escape the while true loop once all the criteria has been met
            break;
        }
        // Protects against equal to and below 0, and numbers above 8
        printf("Invalid number. Acceptable number range: 1-8. Please try again!\n");
    }

    // Init strings to printf with for loop, ensuring to add +1 to account for null terminator
    // This allows us to change the symbols used quickly without having to modify the actual loops
    char blankchar[2] = " ";
    char hashchar[2] = "#";
    char newline[2] = "\n";
    char seperator[3] = "  ";

    for (int i = 0; i < height; i++)
    {
        // Handles printing blank spaces
        // Prints a blank space based on the number of height input minus 1 (to allow for position 1 to have a hash)
        for (int blank = height - 1; blank > i; blank--)
        {
            printf("%s", blankchar);
        }

        // Handles printing hashes
        // Uses -1 to account for the blank spaces we just inserted
        for (int hash = -1; hash < i; hash++)
        {
            printf("%s", hashchar);
        }

        // Add a seperator at the end of each line, which should be two spaces
        printf("%s", seperator);

        // Repeat insertion, but run only the hashchar, as we don't need redundant whitespaces at the end of the output
        for (int althash = -1; althash < i; althash++)
        {
            printf("%s", hashchar);
        }

        // Finally, add a newline at the end of the sequence, rinse and repeat until the height is reached
        printf("%s", newline);
    }
}