/**
 * mario.c
 * 
 * Ryan Boone
 * falldowngoboone@gmail.com
 *
 * Builds a pyramid based on a given height.
 */

#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    
    // Get the height
    do
    {
        printf("Height: ");
        height = GetInt();
    }
    while (height < 0 || height > 23);
    
    // Build the pyramid
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < height + 1; j++)
        {
            if (j + i < height - 1)
            {
                printf(" ");
            }
            else
            {
                printf("#");
            }
        }        
        printf("\n");
    }
}
