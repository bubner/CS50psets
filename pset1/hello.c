#include <stdio.h>
#include <cs50.h>

int main(void)
{
    string name = get_string("What is your name?\n");
    // Concatenate the name and the hello phrase, using the %s placeholder for string placeholder and pass the string to concatenate successfully
    printf("Hello %s!\n", name);
}