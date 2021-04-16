#include "my_tar.h"

int _strlen(char *string)
{
    int i;
    for (i = 0; string[i]; i++);
    return i;
}


void _strcpy(char *destinantion, char *source)
{
    int i;
    for(i = 0; source[i] != '\0'; i++)
    {
        destinantion[i] = source[i];
    }
}

int _strcmp(char *string1, char *string2)
{
    for(; *string1 && *string2 && *string1 == *string2; string1++, string2++);
    return *string1 - *string2;
}

char* _strcat(char* string1, char* string2) {
    int i, j = 0;

    while (string1[i] != '\0')
        ++i;
    while (string2[j] != '\0') {
        string1[i] = string2[j];
        ++j, ++i;
    }
    string1[i] = '\0';
    return string1;
}

unsigned int octal2uint(char *octal, unsigned int size)
{
    unsigned int result = 0;
    unsigned int i = 0;
    while ((i < size) && octal[i])
    {
        result = (result << 3) | (unsigned int) (octal[i++] - '0');
    }
    return result;
}