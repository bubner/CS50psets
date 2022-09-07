
def main():
    height = ""
    # Validate input
    while not height:
        height = input('Height: ')
        if height not in ["1", "2", "3", "4", "5", "6", "7", "8"]:
            height = ""

    # Run for each layer
    height = int(height)
    for i in range(height):
        # Print first section of blank spaces
        print(" " * (height - 1 - i), end="")
        # Print hashes for first section
        print("#" * (i + 1), end="")
        # Print seperator, two blank spaces
        print(" " * 2, end="")
        # Print second section of hashes, we don't need the blank space on the end
        print("#" * (i + 1), end="")
        # Newline
        print("\n", end="")
        

if __name__ == "__main__":
    main()