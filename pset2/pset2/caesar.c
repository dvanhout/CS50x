#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//  when run, program takes an integer (variable: key) at command line
//  user inputs text string which the program then encodes using Caesar's cipher method
//  outputs encripted message where each alphabetical character is shifted by the key characters
//  does not shift non-alphabetical characters

void encode(char c, char p);

int main(int argc, string argv[])
{
    
    // check if user entered a value for key at runtime
    if (argc != 2)
    {
        printf("Usage: %s <key>\n", argv[0]);
        exit(1);
    }
    
    // convert string key to an integer
    int key = atoi(argv[1]);
    
    // prompt user to enter a string
    string s = GetString();
    
    // encript increase string by key value, wrap around at 'z' or 'Z'
    for (int i = 0; i < strlen(s); i++)
    {
        encode(s[i], key);
    }
    printf("\n");
    return(0);
}


void encode(char c, char p)
{
    if (isalpha(c))
    {
        if (islower(c))
        {
            c = 'a' + ((c - 'a' + p) % 26);
        }
        else if (isupper(c))
        {
            c = 'A' + ((c - 'A' + p) % 26);
        }
    }
    printf("%c", c);
}