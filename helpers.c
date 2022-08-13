#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    double avg = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Gets the mean of the RGB elements from each pixel and reapplies it back to the image
            // Rounds the number with high precision to prevent any errors
            avg = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.00);
            image[i][j].rgbtRed = (int) avg;
            image[i][j].rgbtGreen = (int) avg;
            image[i][j].rgbtBlue = (int) avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE buffer;
    for (int x = 0; x < width / 2; x++)
    {
        // Splits our image horizontally and rewrites the image inverted
        // Uses half of the width as the other size will be placed in the right spot as well
        for (int i = 0; i < height; i++)
        {
            buffer = image[i][x];
            // Invert the image in the buffer and put it back into the image
            // Similar to the moving water visualisation, instead we insert it backwards
            image[i][x] = image[i][width - x - 1];
            image[i][width - x - 1] = buffer;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE buffer[height][width];
    int red = 0;
    int green = 0;
    int blue = 0;

    // Uses float for rounding accuracy
    float count = 0.00;

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            red = 0;
            blue = 0;
            green = 0;
            count = 0.00;

            // Tests the nearest neighbors in range 3 (-1, 0, 1)
            for (int k = -1; k < 2; k++)
            {
                // Test if it is outside the image height-wise, in which case we skip
                if (j + k < 0 || j + k > height - 1)
                {
                    continue;
                }

                for (int x = -1; x < 2; x++)
                {
                    // Test if it is outside the image width-wise, in which case we skip
                    if (i + x < 0 || i + x > width - 1)
                    {
                        continue;
                    }

                    // Append new values to be used
                    red += image[j + k][i + x].rgbtRed;
                    green += image[j + k][i + x].rgbtGreen;
                    blue += image[j + k][i + x].rgbtBlue;
                    // Use an average runs count to even out the blur
                    count++;
                }
            }

            // Apply the blur to a temporary table where we know the colour values have not been modified or influenced
            buffer[j][i].rgbtRed = round(red / count);
            buffer[j][i].rgbtGreen = round(green / count);
            buffer[j][i].rgbtBlue = round(blue / count);
        }
    }

    // Appends these values to the image after everything has been completed
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            image[y][x].rgbtRed = buffer[y][x].rgbtRed;
            image[y][x].rgbtGreen = buffer[y][x].rgbtGreen;
            image[y][x].rgbtBlue = buffer[y][x].rgbtBlue;
        }
    }
    return;
}

// Caps input at 255, with a shorthand if statement
int cap255(int value)
{
    return (value >= 255) ? 255 : value;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE buffer[height][width];

    // Create the Gx and Gy matrices
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Run loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Resets values on each run for each pixel
            // Tests red blue and green channels
            int gxr = 0, gxg = 0, gxb = 0, gyr = 0, gyg = 0, gyb = 0;

            // Tests the pixels in range -1, 3 relative to itself in both x and y directions
            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    // Bounds detection
                    if ((i + k < 0 || i + k > height - 1) || (j + l < 0 || j + l > width - 1))
                    {
                        continue;
                    }

                    // Append channel values for each pixel in a 3x3 radius to the temporary resetting variables
                    // Uses the Sobel edge detection algorithm and multiplies the channel value by the Gx and Gy values
                    gxr += image[i + k][j + l].rgbtRed * Gx[k + 1][l + 1];
                    gxg += image[i + k][j + l].rgbtGreen * Gx[k + 1][l + 1];
                    gxb += image[i + k][j + l].rgbtBlue * Gx[k + 1][l + 1];
                    gyr += image[i + k][j + l].rgbtRed * Gy[k + 1][l + 1];
                    gyg += image[i + k][j + l].rgbtGreen * Gy[k + 1][l + 1];
                    gyb += image[i + k][j + l].rgbtBlue * Gy[k + 1][l + 1];
                }
            }
            // After the 3x3 has been checked and values recorded, append them to a temporary rgbtriple array
            // Rounded capped at 255 square root of gx ^ 2 + gy ^ 2
            buffer[i][j].rgbtRed = cap255(round(sqrt(pow(gxr, 2) + pow(gyr, 2))));
            buffer[i][j].rgbtGreen = cap255(round(sqrt(pow(gxg, 2) + pow(gyg, 2))));
            buffer[i][j].rgbtBlue = cap255(round(sqrt(pow(gxb, 2) + pow(gyb, 2))));
        }
    }

    // Once the buffer is finished, append it to the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = buffer[i][j].rgbtRed;
            image[i][j].rgbtGreen = buffer[i][j].rgbtGreen;
            image[i][j].rgbtBlue = buffer[i][j].rgbtBlue;
        }
    }

    return;
}
