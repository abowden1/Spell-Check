/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

#define TABLE_SIZE 200000

typedef struct node
{
    char *word;
    struct node *next;
} node;

// Word counter variable
int counter;
char word[LENGTH + 1];

// creates a variable to store the provided word to check
char to_chk[LENGTH + 1];

// Declaration of hashtable
node* hashtable[TABLE_SIZE] = {NULL};

// Function to hash words in dictionary
// Function - "djb2" retrieved from http://www.cse.yorku.ca/~oz/hash.html
unsigned long hash(char *str)
    {
        unsigned long hash = 5381;
        int c;

        while ((c = *str++))
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        
        return hash % TABLE_SIZE;
    }

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // stores the lenght of the word to check
    int len = strlen(word);
    
    // ensures it is lowercase to match the hashed dicitonary values
    for (int l = 0; l < len; l++)
    {
        to_chk[l] = tolower(word[l]);
    }
    
    to_chk[len] = '\0';
    
    // hashes current word to check
    int hash_val = hash(to_chk);
    
    // creates node to check against word to check

    node* my_node = hashtable[hash_val];

    // checks our hash table for the provided word, if there is a value in that array index
    while(my_node != NULL)
    {
        if(strcmp( to_chk, my_node->word) == 0)
        {
            return true;
        }
        else
        {
            my_node = my_node->next;
        }
    }
    // returns false if the value doesn't exist
    return false;
    
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // opens dictionary
    FILE* fp = fopen(dictionary, "r");
    
    // error check
    if (fp == NULL)
    {
        printf("Failed to open dictionary");
        return false;
    }
    
    // scans provided dictionary until the end of the file
    while(fscanf(fp, "%s\n", word) != EOF)
    {
        //creates a node to store the scanned word and a pointer to the next word
        node* temp = malloc(sizeof(node));
        memset(temp, 0, sizeof(node));
        temp->word = malloc(strlen(word) + 1); 
        strcpy(temp->word, word);
        
        // hashes current word and provides an int value
        int hash_val = hash(word);
        
        counter++;
        
        // adds hash_val to the appropriate place in the table
        if(hashtable[hash_val] == NULL)
        {
            hashtable[hash_val] = temp;
        }
        
        else
        {
            temp->next = hashtable[hash_val];
            hashtable[hash_val] = temp;
        }
    }
    fclose(fp);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return counter;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    node *loc, *temp;

    for(int i = 0; i < TABLE_SIZE; i++)
    {
        
        loc = hashtable[i];
        while (loc != NULL)
        {
            //node* temp = loc->next;
            free(loc->word);
            temp = loc->next;
            free(loc);
            loc = temp;
        }

    }
    return true;
}
