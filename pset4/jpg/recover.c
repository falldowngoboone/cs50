/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t  BYTE;

int main(int argc, char* argv[])
{
    // track the image number
    int counter = 0;
    
    // data buffer
    BYTE data[512];
    
    // open memory card file
    FILE* card = fopen("card.raw", "r");
    
    // initialize the image file pointer
    FILE* img = NULL;
    
    // handle if card is NULL
    if (card == NULL)
    {
        printf("Could not open card file.\n");
        return 1;
    }
    
    // loop through data until the end of file
    while (fread(&data, 512, 1, card) == 1)
    {
        // a new JPEG is starting
        if (data[0] == 0xff && data[1] == 0xd8 && 
            data[2] == 0xff && (data[3] == 0xe0 || data[3] == 0xe1))
        {
            // close any open files
            if (img != NULL)
            {
                fclose(img);
            }
            
            // create a buffer for the title
            char* title = malloc(50 * sizeof(char));
            
            // title with 3-space padding formatted as zeroes
            sprintf(title, "%03d.jpg", counter);
            
            // open a new JPEG file
            img = fopen(title, "w");

            if (card == NULL)
            {
                // clean up memory and return an error code
                printf("Could not open the file %s.\n", title);
                free(title);
                return 2;
            }
            
            // clean up memory
            free(title);
            
            // increment counter
            counter++;
        }
        // if an image is open...
        if (img != NULL)
        {
            // write to the file
            fwrite(&data, 512, 1, img);
        }
        
    }
    
    // if there's an image, close it
    if (img != NULL)
    {
        fclose(img);
    }
    
    // finally, close the card file
    fclose(card);
    
    // done
    return 0;
}
