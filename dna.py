import csv
import sys


def main():
    # Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    # Read database and sequence files into variables
    with open(sys.argv[1], 'r') as datafile:
        with open(sys.argv[2], 'r') as seqfile:

            # Interpret data on files
            data = csv.DictReader(datafile)
            sequence = seqfile.readline().strip()

            # Find longest match of each STR in DNA sequence
            # Slice first value (name) out of dictionary
            matches = {}
            for dnasequence in data.fieldnames[1:]:
                # Only allows a match to enter the matches database if it has a chain of at least 2
                matches[dnasequence] = longest_match(sequence, dnasequence) if longest_match(sequence, dnasequence) >= 2 else 0

            # Check database for matching profiles
            # For every person, check their STR matches against the counted ones
            # The one with most matches, and the one that fits the matching criteria, will be returned to the user
            finds = {}
            for person in data:
                finds[person['name']] = 0
                for i in data.fieldnames[1:]:
                    if int(matches[i]) == int(person[i]):
                        finds[person['name']] += 1

            # Print the match that corresponds to the number of DNA entries occupied, otherwise print No match
            print(determine_owner(finds, data, matches))


def determine_owner(subjects, data, matches):
    # Extract key and value pairs and initialise variables
    subkeys = list(subjects.keys())
    subvals = list(subjects.values())
    length = 0

    # Determine how many STR samples are actually present
    for i in data.fieldnames[1:]:
        if matches[i] != 0:
            length += 1

    # If there are 0 matched samples, or the subjects don't match the number of samples, return no match
    if not length == max(subvals) or all(value == 0 for value in subvals):
        return "No match"

    # Otherwise, return the person with the highest match count
    return subkeys[subvals.index(max(subvals))]


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


if __name__ == "__main__":
    main()
