/**
 * resize.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Ryan Boone
 * falldowngoboone@gmail.com
 *
 * Resizes a bitmap.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }
    
    // cache factor and filenames
    int factor_n = atoi(argv[1]);
    char* infile = argv[2];
    char* outfile = argv[3];

    if (factor_n < 1 || factor_n > 100)
    {
        printf("Factor n must be positive and less than or equal to 100!\n");
        return 2;
    }

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 3;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 4;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 5;
    }
    
    // copy bf
    BITMAPFILEHEADER new_bf = bf;

    // copy bi
    BITMAPINFOHEADER new_bi = bi;
    
    // update new BITMAPINFOHEADER
    new_bi.biWidth *= factor_n;
    new_bi.biHeight *= factor_n;
    
    // determine outfile padding for new biSizeImage
    int outfile_padding = (4 - (new_bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    new_bi.biSizeImage = (new_bi.biWidth * sizeof(RGBTRIPLE) + outfile_padding) * abs(new_bi.biHeight);
    
    // update new BITMAPFILEHEADER
    new_bf.bfSize = new_bi.biSizeImage + new_bf.bfOffBits;
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&new_bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    
    // write outfile's BITMAPINFOHEADER
    fwrite(&new_bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine infile padding for scanlines
    int infile_padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // cache an entire line somehow or rewind the read pointer every iteration
    
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // repeat line scan n times
        int scan;
        
        for (scan = 0; scan < factor_n; scan++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple, 
                          triple_x_n[factor_n];

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                
                // increase width of triple by factor n
                for (int l = 0; l < factor_n; l++)
                {
                    triple_x_n[l] = triple;
                }

                // write RGB triple to outfile n times
                fwrite(&triple_x_n, sizeof(RGBTRIPLE) * factor_n, 1, outptr); 
            }
            
            // add the new padding
            for (int k = 0; k < outfile_padding; k++)
            {
                fputc(0x00, outptr);
            }

            if (scan < factor_n - 1)
            {
                // rewind the infile pointer except the last scan
                fseek(inptr, -bi.biWidth * sizeof(RGBTRIPLE), SEEK_CUR);
            }
            else
            {
                // else, skip over padding, if any
                fseek(inptr, infile_padding, SEEK_CUR);
            }
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
