/**
 * vigenere.c
 *
 * Ryan Boone
 * falldowngoboone@gmail.com
 *
 * Prints an encrypted string using a Vigenere cipher.
 */

#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

string Vigenere(string secret_msg, string key);

int main(int argc, string argv[])
{   
    // Only one argument allowed
    if (argc != 2)
    {
        printf("Please give one string for the key!\n");
        return 1;
    }

    string key = argv[1];
    
    // Key must be alphabetical, all lower-case
    for (int i = 0, n = strlen(key); i < n; i++)
    {
        if (!isalpha(key[i]))
        {
            printf("Only alphabetical chars, please!\n");
            return 1;
        }
        else
        {
            key[i] = tolower(key[i]);
        }
    }
    
    string message = GetString();
    string enc_msg = Vigenere(message, key);
    
    printf("%s\n", enc_msg);
    
    return 0;
}

/**
 * Encrypts a string with a Vigenere cipher
 *
 * @param  {string} secret_msg - the string to be encrypted
 * @param  {string} key - the string used for cipher encoding
 * @return (string} result - the result of the encoding
 */
string Vigenere(string secret_msg, string key)
{
    string result = secret_msg;
    
    // Track length and index of key
    int key_mod = strlen(key), 
        key_index = 0;
    
    for (int i = 0, n = strlen(result); i < n; i++)
    {
        if (isalpha(result[i]))
        {
            char key_letter = key[key_index % key_mod] - 'a';
            int offset = isupper(result[i]) ? 'A' : 'a';
            result[i] = (((result[i] - offset) + key_letter) % (26)) + offset;
            
            // Character is alphabetic, so increment key_index
            key_index++;
        }
    }
    
    return result;
}
