#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
            // Added i-- otherwise the loop would count the invalid input as a valid vote even though it's been checked as not
            i--;
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string input)
{
    for (int i = 0; i < candidate_count; i++)
    {
        // Uses strcmp to compare strings as == doesn't work
        if (strcmp(input, candidates[i].name) == 0)
        {
            // Increment their vote by one if it is correct
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // Adds a top vote buffer to determine the highest
    int topvotes = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        // Runs some conditions to determine the top of all time, unless it is equal to the top in which case we add that to another variable
        if (candidates[i].votes > topvotes)
        {
            // Makes sure the highest vote is recorded
            topvotes = candidates[i].votes;
        }
    }

    // Check for any candidates with the same amount of votes as the winning amount and print them (accounts for dupes)
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == topvotes)
        {
            printf("%s\n", candidates[i].name);
        }
    }
    return;
}