/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember resize scale
    int n = atoi(argv[1]);
    
    // ensure resize is > 0 and <= 100
    if ((n > 100) || (n < 0))
    {
        fprintf(stderr, "Usage: n must be a positive integer less than or equal to 100\n");
        return 1;
    }
    
    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
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
        return 4;
    }
    
    // Create new BITMAPINFOHEADER and update scale 
    BITMAPINFOHEADER binew = bi;
    binew.biWidth *= n;
    binew.biHeight *= n;

    // determine inputr and outputr padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int paddingnew = (4 - (binew.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    binew.biSizeImage = ((sizeof(RGBTRIPLE) * binew.biWidth) + paddingnew) * abs(binew.biHeight);
    
    // Create new BITMAPFILEHEADER update scale
    BITMAPFILEHEADER bfnew = bf;
    bfnew.bfSize = binew.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bfnew, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&binew, sizeof(BITMAPINFOHEADER), 1, outptr);


    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // temporary storage
        RGBTRIPLE triple;   

        // vertical scaling n - 1 times
        for (int m = 0; m < (n - 1); m++)
        {

            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
            
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                for (int k = 0; k < n; k++)     
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
        
            // add padding to outptr scanline as necessary
            for (int l = 0; l < paddingnew; l++)
                fputc(0x00, outptr);

            // set cursor back to beginning of scanline
            int rgb = sizeof(RGBTRIPLE);
            fseek(inptr, -bi.biWidth * rgb, SEEK_CUR);

        }

        // iterate over pixels in scanline one more time
        for (int j = 0; j < bi.biWidth; j++)
        {
            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            for (int k = 0; k < n; k++)     
                // write RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }
        
        // add padding to outptr scanline as necessary
        for (int l = 0; l < paddingnew; l++)
        fputc(0x00, outptr);
        
        // skip over infile padding to next scanline
        fseek(inptr, padding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
