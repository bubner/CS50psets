
def main():
    # Parse input
    text = ""
    while not text:
        text = input("Text: ")

    letters = sentences = 0
    # Words is always off by 1, so we start at 1 to counter
    words = 1

    # Get text properties
    for letter in text:
        if letter.isalpha():
            letters += 1
        if letter.isspace():
            words += 1
        if letter == "." or letter == "?" or letter == "!":
            sentences += 1

    # Grade text based on these properties
    AVERAGE_LETTERS_PER_100_WORDS = letters / words * 100
    AVERAGE_SENTENCES_PER_100_WORDS = sentences / words * 100

    # Utilises the Coleman-Liau index
    index = round(0.0588 * AVERAGE_LETTERS_PER_100_WORDS - 0.296 * AVERAGE_SENTENCES_PER_100_WORDS - 15.8)

    # Report index grade to the user
    if index <= 1:
        print("Before Grade 1")
    elif index >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {index}")


if __name__ == "__main__":
    main()