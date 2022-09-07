// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// 65535 buckets for hashing function, utilising exponential bitwise operation hashing
const unsigned int N = 65535;
// Global variable to count words in the dictionary easily / with speed
unsigned int words = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Hash the input word
    unsigned int hashVal = hash(word);

    // Create a node traveller
    node *traveller = table[hashVal];

    // Loop over the linked list until we reach the end (NULL)
    while (traveller != NULL)
    {
        // Compare strings to see if the word is there or not
        if (strcasecmp(word, traveller->word) == 0)
        {
            return true;
        }

        // Move to the next node if it does not match
        traveller = traveller->next;
    }
    // Return false if we run out of words and no match was found
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Speed hash function optimised from jagottsicher on StackOverflow to work with lower and uppercase letters
    unsigned int h = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        // Bitwise incrementation
        h = (h << 2) ^ tolower(word[i]);
    }
    // Return final hash against bucket count
    return h % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open the dictionary into memory
    FILE *dict = fopen(dictionary, "r");

    // If this operation fails, report false
    if (dict == NULL)
    {
        return false;
    }

    // Read strings from file, until fscanf stops reporting 1
    char string[LENGTH + 1];
    while (fscanf(dict, "%s", string) == 1)
    {
        // Make a new node and memory for the word
        node *buffer = malloc(sizeof(node));
        // Check if the memory is allocated without errors
        if (buffer == NULL)
        {
            return false;
        }

        // Get the dictionary word and put it into the word
        strcpy(buffer->word, string);

        int hashed = hash(string);

        // Check head and where it is pointing and point buffer to NULL if the head is
        if (table[hashed] == NULL)
        {
            buffer->next = NULL;
        }
        else
        {
            // If not, point buffer to the nodes in the linked list
            buffer->next = table[hashed];
        }

        // Move the head to the buffer
        table[hashed] = buffer;

        // Increment word count global variable
        words++;
    }

    // Close file and return true
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return words;
}

void nfree(node *n)
{
    // Recursive function to clear linked list with following nodes
    if (n->next != NULL)
    {
        nfree(n->next);
    }

    // Base case
    free(n);
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Traverse linked list and call to custom nodefree function
    for (int i = 0; i < N; i++)
    {
        // Checks if we've reached the end of the linked list
        if (table[i] != NULL)
        {
            nfree(table[i]);
        }
    }
    // Return true to indicate we've cleared what we can
    return true;
}
