// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // Grab the header and put it in the output file
    uint8_t header[HEADER_SIZE];
    fread(header, HEADER_SIZE, 1, input);
    fwrite(header, HEADER_SIZE, 1, output);

    // Read the samples from the input file and write them to the output file with the modification from the factor
    // Use two byte samples
    int16_t samples;
    while (fread(&samples, sizeof(int16_t), 1, input))
    {
        // Multiply the factor by the sample, updating the volume
        samples *= factor;
        fwrite(&samples, sizeof(int16_t), 1, output);
    }


    // Close files
    fclose(input);
    fclose(output);
}
