#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Parse command line input
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Load card into memory
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Could not read file '%s'", argv[1]);
        return 2;
    }

    // Initalise variables
    FILE *image;
    char fname[8];
    unsigned char *buffer = malloc(512);
    int i = 0;

    // Read 512 byte chunks until we run out
    while (fread(buffer, 512, 1, card))
    {
        // If we find the four chunks of hexadecimal are towards the JPEG sequence, then write a new file
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // Close the old file to prevent memory issues, unless it is the first or last in which they don't need to be looped
            if (i != 0)
            {
                fclose(image);
            }

            // Make a new file for the newly detected JPEG
            sprintf(fname, "%03d.jpg", i);

            // Open the new file in write mode
            image = fopen(fname, "w");

            // Failsafe
            if (image == NULL)
            {
                fclose(card);
                free(buffer);
                printf("Could not make output file '%s'", fname);
                return 3;
            }

            // If all these checks pass, then increment the number of images we need to construct (file naming + detection)
            i++;
        }

        // Finally, if we know that there are images in the card, we can write 512 byte chunks to each of the files
        if (i > 0)
        {
            fwrite(buffer, 512, 1, image);
        }
    }

    // Clean up after we're done and return 0 for success
    free(buffer);
    fclose(image);
    fclose(card);
    return 0;
}