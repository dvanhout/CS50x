#include <stdio.h>
#include <string.h>
#include <cs50.h>

// program gets a string from the user and ouputs initials in upper case
// string may contain any characters
// method: look for spaces and capitalize next character if it's alphabetical

void capitalize(char c); 

int main (void)
{
    // get a string from user
    string s = GetString();   
    
    // capitalize the first character of the string
    capitalize(s[0]); 
    
    // search for ' ' and capitalize the next char after
    for (int i = 0; i < strlen(s); i++)  
    {
        if (s[i] == ' ') 
        {
            capitalize(s[i+1]);
        }

    }
    printf("\n");
}

// takes a char and prints it in upper case
void capitalize (char c) 
{
    if ((c <= 122) && (c >= 97))
    {
        printf("%c", c-32);
    }
    else
    {
        printf("%c", c);
    }

}
