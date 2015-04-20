#include "Print.h"
#include "Initialization.h"

void printInputVector(char * input)
{
    int i;

    for(i = 0; i < Npi; i++)
    {
        printf("%c", input[i]);
    }

    printf("\n");
}

void printPattern(int *pattern)
{
    int i;

    for(i = 0; i < Npi; i++)
    {
        printf("%d", pattern[i]);
    }

    printf("\n");
}
