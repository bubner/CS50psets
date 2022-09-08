#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// Runs given strings against the Coleman-Liau index and returns a grade from <2-16+

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    string text = get_string("Text: ");
    // Count the letters, words, and sentences and save them to memory
    const int letters = count_letters(text);
    const int words = count_words(text);
    const int sentences = count_sentences(text);

    // Calculate the grade based on the formula index = 0.0588 * L - 0.296 * S - 15.8
    // Utilises constants as these values should never change
    const float AVERAGE_LETTERS_PER_100_WORDS = (float) letters / (float) words * 100;
    const float AVERAGE_SENTENCES_PER_100_WORDS = (float) sentences / (float) words * 100;
    const int output = round(0.0588 * AVERAGE_LETTERS_PER_100_WORDS - 0.296 * AVERAGE_SENTENCES_PER_100_WORDS - 15.8);

    // Test if the output is below 1 or above 16 and prints the respective comment
    if (output <= 1)
    {
        printf("Before Grade 1\n");
    }
    else if (output >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", output);
    }
}

// Counts letters in a given string and returns them
int count_letters(string text)
{
    int output = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        // Tests if it is alphabetical, and adds to the output if it is
        if (isalpha(text[i]))
        {
            output += 1;
        }
    }
    return output;
}

// Counts words in a given string and returns them
int count_words(string text)
{
    // This function returns 1 below expected value, likely due to the missing of a character, so we simply add one to the output as it is always off by one
    int output = 1;
    for (int i = 0; i < strlen(text); i++)
    {
        // Counts all the spaces in the text, which indicates a new word
        if (isblank(text[i]))
        {
            output += 1;
        }
    }
    return output;
}

// Counts sentences in a given string and returns them
int count_sentences(string text)
{
    int output = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        // Checks if the character is a full stop, question or exclamation mark and adds one to the output
        if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            output += 1;
        }
    }
    return output;
}