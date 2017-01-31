/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "dictionary.h"

// linked list nodes
typedef struct node
{
    char word[LENGTH + 1];
    struct node* next;
}
node;

// dictionary hashtable
typedef struct Collection
{
    int size;
    node* hashtable[HASHTABLE_SIZE];
}
Collection;

Collection collection;

/**
 * Returns an int to set a hash table
 *
 * Courtesy deliptiy's husband (on Reddit):
 * http://www.reddit.com/r/cs50/comments/1x6vc8/pset6_trie_vs_hashtable/
 *
 * bitwise operators FTW!!!
 */
int hash_word(const char* word)
{
    unsigned int hash = 0;
    for (int i=0, n=strlen(word); i < n; i++)
    {
        hash = (hash << 2) ^ word[i];
    }
    return hash % HASHTABLE_SIZE;
}

/**
 * Inserts a word into a new node
 */
bool insert(node* new_node)
{
    // hash the word
    int hash = hash_word(new_node->word);
    
    // insert the word into collection.hashtable[hash]
    new_node->next = collection.hashtable[hash];
    collection.hashtable[hash] = new_node;
    
    // increment the hashtable's length
    collection.size = collection.size + 1;
    
    return true;
};

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // make word lowercase
    char ch_word[LENGTH + 1];
    int len = strlen(word);
    for (int i = 0; i < len; i++)
    {
        ch_word[i] = tolower(word[i]);
    }
    ch_word[len] = '\0';
    
    // hash word
    int hash = hash_word(ch_word);
    
    // search the hashtable =)
    node* cursor = collection.hashtable[hash];
    
    while (cursor != NULL)
    {
        if (strcmp(cursor->word, ch_word) == 0)
        {
            // matched! =)
            return true;
        }
        else
        {
            // not a match, go on to the next one =(
            cursor = cursor->next;
        }
    }
    
    // no matches at all
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // buffer for fgets
    char word[LENGTH + 1];
    
    // initialize the collection hashtable size
    collection.size = 0;
    
    // open dictionary
    FILE* fp = fopen(dictionary, "r");
    if (fp == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }
    
    while (fscanf(fp, "%s", word) == 1)
    {   
        
        // malloc a node* for each new word
        node* new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            printf("Could not write %s to dictionary.\n", word);
            return false;
        }
        
        // assign word to new_node
        strcpy(new_node->word, word);
        
        new_node->next = NULL;
        
        // insert word into hash table
        insert(new_node);
    }
    
    // close dictionary
    fclose(fp);
    
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return collection.size;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // node we'll crawl the linked lists with
    node* cursor;
    
    // crawl through all nodes on the hashtable
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        cursor = collection.hashtable[i];
        while (cursor != NULL)
        {
            node* temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    
    return true;
}
