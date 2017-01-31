/**
 * caesar.c
 *
 * Ryan Boone
 * falldowngoboone@gmail.com
 *
 * Prints an encrypted string using a Caesar cipher.
 */

#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

string Caesar(string secret_msg, int key);

int main(int argc, string argv[])
{   
    // Check quality of args
    if (argc != 2
        || (atoi(argv[1]) == 0 && strcmp(argv[1], "0") != 0)
        || atoi(argv[1]) < 0)
    {
        printf("Please give one non-negative integer for the key!\n");
        return 1;
    }

    int key = atoi(argv[1]);
    string message = GetString();
    
    string enc_msg = Caesar(message, key);
    
    printf("%s\n", enc_msg);
    
    return 0;
}

/**
 * Encrypts a string with a Caesar cipher
 *
 * @param  {string} secret_msg - the string to be encrypted
 * @param  {int}    key - non-negative int used for cipher encoding
 * @return (string} result - the result of the encoding
 */
string Caesar(string secret_msg, int key)
{
    string result = secret_msg;
    
    for (int i = 0, n = strlen(result); i < n; i++)
    {
        if (isalpha(result[i]))
        {
            int offset = isupper(result[i]) ? 'A' : 'a';
            result[i] = (((result[i] - offset) + key) % 26) + offset;
        }
    }
    
    return result;
}
