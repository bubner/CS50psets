#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // Print the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    // Make a variable to store our score
    int score = 0;

    // Run against dictionary
    for (int i = 0; i < strlen(word); i++)
    {
        // Test if the letter is uppercase, if so then use ASCII 65 which is captial A
        // Otherwise do the same with lowercase but use ASCII 97 as it is lowercase
        if (isupper(word[i]))
        {
            // Minus ASCII 65 = upper A to append the correct letter from the word index
            score += POINTS[word[i] - 65];
        }
        else if (islower(word[i]))
        {
            // ASCII 97 = lowercase A
            score += POINTS[word[i] - 97];
        }
    }

    // Return the score
    return score;
}
