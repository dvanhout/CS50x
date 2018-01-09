/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

#define HASHSIZE 27

// define hash table
typedef struct node
{
    char *word;
    struct node* next;
} node;

node *hash_table[HASHSIZE];

// track number of words in the dictionary
int wordcount = 0;

// define global variable
char buffer[LENGTH + 1];


// function declaration
int hash(const char *str); 


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // determine where word should fit into hash table
    int hashvalue = hash(word);
    
    // set a pointer to traverse hash table
    node *ptr = hash_table[hashvalue];

    // if pointer is null then then the word is not in the bucket

    // traverse the linked list to see if word is there
    while (ptr != NULL)
    {
        
        if (strcasecmp(ptr->word, word) == 0)
            // word is in the list
            return true;

        ptr = ptr->next;
    }
    // word was not encountered
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // open the dictionary
    FILE* fp1 = fopen(dictionary, "r");
    
    // check if the dictionary is empty
    if (fp1 == NULL)
        return false;
    
    // traverse dictionary until end and store string in buffer variable
    while (fscanf(fp1, "%s/n", buffer) != EOF)
    {
        // Allocate memory for new node pointer
        node* new_node = malloc(sizeof(node));
        
        // Check if malloc loaded new_node correctly
        if (new_node == NULL)
        {
            unload();
            return false;
        }

        // allocate memory for data and copy buffer in 
        new_node->word = malloc(strlen(buffer) + 1);
        strcpy(new_node->word, buffer);
        
        // call to hash function to determine which bucket
        int bucket = hash(buffer);
        
        // determine where new_node belongs and add    
        if (hash_table[bucket] == new_node)
        {
            hash_table[bucket] = new_node;
            new_node->next = NULL;
        }
        else
        {
            new_node->next = hash_table[bucket];
            hash_table[bucket] = new_node;
        }
        
        // count new word in total
        wordcount++;
                    
    }
    // close the file
    fclose(fp1);
    
    // success!
    return true;
    
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // simply return wordcount determined in load function
    return wordcount;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // go to each bucket
    for (int i = 0; i <= HASHSIZE; i++)
    {
    
        // point cursor to current bucket
        node *cursor = hash_table[i];
        
        // traverse list and free memory
        while (cursor != NULL)
        {
            node *ptr = cursor;
            cursor = cursor->next;
            free(ptr);
            return true;
        }
    
    // set buckets to NULL
    hash_table[i] = NULL;
    }
    
    // success
    return true;
}



// thanks to fellow, former cs50 student https://github.com/edouardjamin/CS50/blob/master/pset5/dictionary.c for hash function idea

int hash(const char *str) 
{
    // initialize index to 0
    int index = 0;

    // sum ascii values
    for (int i = 0; str[i] != '\0'; i++)
        // search for lower cases words
        index += tolower(str[i]);

    // mod by size to stay w/in bound of table
    return index % HASHSIZE;
}
