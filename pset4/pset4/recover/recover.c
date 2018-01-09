#include <stdio.h>
#include <stdint.h>

#define CHUNK 512

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }
    
    // open input file 
    FILE *infile = fopen(argv[1], "r");

    // check for content
    if (infile == NULL)
    {
        fprintf(stderr, "Could not open file.\n");
        return 2;
    }

    // create a counter for filenames
    int i = 0;
    
    // file pointer to ouput file
    FILE* outfile = NULL;

    // create a buffer
    BYTE buffer[CHUNK];
 
    while (fread(&buffer, sizeof(buffer), 1, infile) == 1)    
    {
        // check if byte sequence are jpgs
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // close previous file if one is open
            if (i > 0)
                fclose(outfile);
                
            // create temporary array for filename storage
            char TEMPFILE[8];

            // print filename into temp storage
            sprintf(TEMPFILE, "%03d.jpg", i);
            
            // name the outfile and open for writing
            outfile = fopen(TEMPFILE, "w");
            i++;
        }

        // write buffer to outfile
        if (i > 0)
            fwrite(&buffer, sizeof(buffer), 1, outfile);
        
    }
    // close outfile
    fclose(outfile);
    
    // close infile 
    fclose(infile);
}