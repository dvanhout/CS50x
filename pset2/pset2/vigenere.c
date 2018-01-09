#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// program uses Vigenere method to encript a string
// e.g. user inputs: key = 'abCD', and string = 'This is Awesome!'...
// ...outputs encription: 'Tikv it Czetqpe!'
// key must only be alphabetical
// string can contain any character

void encode(char c, char p);

int main(int argc, string argv[])
{
    string key = argv[1];
        
    // check if user entered a value for key
    if (argc != 2)
    {
        printf("Usage: %s <key>\n", argv[0]);
        exit(1);
    }

    // check that key is a string of alpha chars
    for (int a = 0; a < strlen(key); a++)
    {
        if (!isalpha(key[a]))
        {
            printf("Usage: %s <letters only key>\n", argv[0]);
            exit(1);
        }
    }

    // prompt user to enter a string to encode
    string s = GetString();

    int j = 0;

    // encript alpha chars of s using key 
    for (int i = 0; i < strlen(s); i++)
    {
        encode(s[i], key[j % strlen(key)]);
        
        if (isalpha(s[i]))
        {
            j++;
        }
    }
    printf("\n");
    return(0);
}

// check if char c is alphabetical
// if p is uppercase, convert to lowercase in ASCII
// increase c gets value of 
void encode(char c, char p)
{
    if (isalpha(c))
    {
        if (isupper(p))
        {
            p = p + 32;
        }
        
        if (islower(c))
        {
            c = 'a' + ((c - 'a' + p - 'a') % 26);
        }
        
        else if (isupper(c))
        {
            c = 'A' + ((c - 'A' + p - 'a') % 26);
        }

    }
    printf("%c", c);
}