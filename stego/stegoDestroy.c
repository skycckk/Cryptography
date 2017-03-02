//
// Stego --- destroy info in a BMP image file
//

#include "stego.h"

int main(int argc, const char *argv[])
{
    FILE *in,
         *out;
    
    int i,
        j,
        x,
        ttt,
        shft,
        byteCount,
        moreData,
        moreImage,
        imageBytes,
        dataBytes;

    char temp,
         data;

    char infname[80],
         outfname[80];

    if(argc != 3)
    {
oops:   fprintf(stderr, "\nUsage: %s plainImage stegoImage stegoData\n\n", argv[0]);
        fprintf(stderr, "where plainImage == filename containing initial image\n");
        fprintf(stderr, "      stegoImage == filename for image containing stego data\n");
        exit(0);
    }
    
    sprintf(infname, "%s", argv[1]);
    sprintf(outfname, "%s", argv[2]);

    in = fopen(infname, "r");
    if(in == NULL)
    {
        fprintf(stderr, "\nError opening file %s\n\n", infname);
        exit(0);
    }

    imageBytes = 0;
    while(1)
    {
        x = fscanf(in, "%c", &temp);
        if(x != 1)
        {
            break;
        }
        ++imageBytes;
    }
    fclose(in);
    
    printf("image bytes = %d, capacity = %d bytes\n", imageBytes, imageBytes >> 3);
    
    in = fopen(infname, "r");
    if(in == NULL)
    {
        fprintf(stderr, "\nError opening file %s\n\n", infname);
        exit(0);
    }    

    out = fopen(outfname, "w");
    if(out == NULL)
    {
        fprintf(stderr, "\nError opening file %s\n\n", outfname);
        exit(0);
    }

    //
    // skip first START_FROM bytes of image file
    //
    for(i = 0; i < START_FROM; ++i)
    {
        x = fscanf(in, "%c", &temp);
        if(x != 1)
        {
            fprintf(stderr, "\nError in file %s\n\n", infname);
            exit(0);
        }
        fprintf(out, "%c", temp);
    }

    //
    // destroy 64 bytes of the form 0xa5
    // which indicates that the file contains stego data
    // (by taking first byte)
    //
    ttt = 0xa5;
    for(i = 0; i < 8; ++i)
    {
        for(j = 0; j < 8; ++j)
        {
            x = fscanf(in, "%c", &temp);
            if(x != 1)
            {
                fprintf(stderr, "\nError in file %s\n\n", infname);
                exit(0);
            }

            if (i == 0 && j == 0) temp = temp & 0xfe;
            fprintf(out, "%c", temp);
        }
    }

    // retrieve number of bits of hidden data
    dataBytes = 0;
    for(i = 0; i < 27; ++i)
    {
        x = fscanf(in, "%c", &temp);
        if(x != 1)
        {
            fprintf(stderr, "\nError in file %s\n\n", infname);
            exit(0);
        }
        dataBytes ^= ((temp & 0x1) << i);
        fprintf(out, "%c", temp);
    }

    // destroy hidden info
    for(i = 0; i < (dataBytes << 3); ++i)
    {
        x = fscanf(in, "%c", &temp);

        if(x != 1)
        {
            fprintf(stderr, "\nError in file %s\n\n", infname);
            exit(0);
        }

        if (i % 2 == 0) temp = temp & 0xfe; // destroy the hidden bit by setting zero
        fprintf(out, "%c", temp);
    }
    //

    // copy the rest part of input
    while (fscanf(in, "%c", &temp) != -1)
    {
        fprintf(out, "%c", temp);
    }
    
    printf("\n");
    fclose(in);
    fclose(out);

}// end main
