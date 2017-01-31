/**
 * initials.c
 *
 * Ryan Boone
 * falldowngoboone@gmail.com
 *
 * Converts a full name to initials.
 */

#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void ToInitials(string name);

int main(void)
{
    string name = GetString();
    ToInitials(name);
    
    return 0;
}

/**
 * Prints a full name's capitalized initials
 * @param {string (char *)} name - any given full name
 */
void ToInitials(string full_name)
{
    if (full_name != NULL)
    {
        printf("%c", toupper(full_name[0]));
        for (int i = 0, n = strlen(full_name); i < n; i++)
        {
            if (isspace(full_name[i]))
            {
                printf("%c", toupper(full_name[i + 1]));
            }
        }
        printf("\n");
    }
}
