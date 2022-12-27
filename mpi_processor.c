#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "image_bright.c"
#include "image_dark.c"
#include "image_colortosepia.c"
#include "image_simulate_cvd.c"
#include "image_correct_cvd.c"
#include "image_rgbtogray.c"
#include "black_white.c"
#include "image_blur_color.c"
#include "image_blur_gray.c"
#include "image_rotate.c"
#include "negative_image.c"
#include "mpi.h"

#ifdef RELEASE
#define printer
#else
#define printer printf
#endif

int main(int argc, char *argv[])
{
    // Initialize the MPI environment
    int numImages = 24;

    char coloredImages[24][100] = {
        "airplane",
        "baboon",
        "barbara",
        "BoatsColor",
        "goldhill",
        "lena_color",
        "airplane",
        "baboon",
        "barbara",
        "BoatsColor",
        "goldhill",
        "lena_color",
        "airplane",
        "baboon",
        "barbara",
        "BoatsColor",
        "goldhill",
        "lena_color",
        "airplane",
        "baboon",
        "barbara",
        "BoatsColor",
        "goldhill",
        "lena_color",
    };
    char ImageFilePath[150];
    FILE *fIn;
    FILE *fIn3D;
    unsigned char header[54];
    unsigned char colorTable[1024];
    int height;
    int width;
    int bitDepth;
    int imgsize;
    unsigned char *buffer, *D3buffer;


    MPI_Init(argc, argv);
    MPI_Comm comm = MPI_COMM_WORLD;
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
    
    for (int imgIndex = 0; imgIndex < numImages; imgIndex++){
        if (rank == 0)
        {
            sprintf(ImageFilePath, "images/%s.bmp", coloredImages[imgIndex]);
            fIn = fopen(ImageFilePath, "r"); // Input File name
            fIn3D = fopen(ImageFilePath, "r"); // Input File name
            for (int i = 0; i < 54; i++)           // read the 54 byte header from fIn
            {
                {
                    header[i] = getc(fIn);
                    getc(fIn3D);
                }
            }
            height = *(int *)&header[18];
            width = *(int *)&header[22];
            bitDepth = *(int *)&header[28];
            if (bitDepth <= 8) // if ColorTable present, extract it.
            {
                fread(colorTable, sizeof(unsigned char), 1024, fIn);
            }

            imgsize = height * width; // calculate image size
            // D3buffer[width][height][3];
            D3buffer = malloc(width * height * 3);
            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < height; j++)
                {
                    D3buffer[i * width + j * 3 + 2] = getc(fIn3D); // blue
                    D3buffer[i * width + j * 3 + 1] = getc(fIn3D); // green
                    D3buffer[i * width + j * 3 + 0] = getc(fIn3D); // red
                }
            }
            buffer = malloc( imgsize * 3 ); // to store the image data

            for (int i = 0; i < size; i++)
            {
                {
                    buffer[i * 3 + 2] = getc(fIn); // blue
                    buffer[i * 3 + 1] = getc(fIn); // green
                    buffer[i * 3 + 0] = getc(fIn); // red
                }
            }

            printer("height: %d\n", height);
            printer("width: %d\n", width);
            printer("imgsize: %d\n", imgsize);

            MPI_Bcast(&height, 1, MPI_INT, 0, comm);
            MPI_Bcast(&width, 1, MPI_INT, 0, comm);
            MPI_Bcast(&bitDepth, 1, MPI_INT, 0, comm);
            MPI_Bcast(&imgsize, 1, MPI_INT, 0, comm);
            MPI_Bcast(&buffer, imgsize * 3, MPI_UNSIGNED_CHAR, 0, comm);
            MPI_Bcast(&D3buffer, width * height * 3, MPI_UNSIGNED_CHAR, 0, comm);
            
            
            
            MPI_Bcast(&header, 54, MPI_UNSIGNED_CHAR, 0, comm);
            MPI_Bcast(&colorTable, 1024, MPI_UNSIGNED_CHAR, 0, comm);


        }
        else
        {
            MPI_Bcast(&height, 1, MPI_INT, 0, comm);
            MPI_Bcast(&width, 1, MPI_INT, 0, comm);
            MPI_Bcast(&bitDepth, 1, MPI_INT, 0, comm);
            MPI_Bcast(&imgsize, 1, MPI_INT, 0, comm);
            D3buffer = malloc(width * height * 3);
            buffer = malloc(imgsize * 3);

            MPI_Bcast(&buffer, imgsize * 3, MPI_UNSIGNED_CHAR, 0, comm);
            MPI_Bcast(&D3buffer, width * height * 3, MPI_UNSIGNED_CHAR, 0, comm);
            MPI_Bcast(&header, 54, MPI_UNSIGNED_CHAR, 0, comm);
            MPI_Bcast(&colorTable, 1024, MPI_UNSIGNED_CHAR, 0, comm);
        }

        image_colortosepia(coloredImages[imgIndex], header, imgsize, buffer, bitDepth, colorTable);

        simulate_cvd_protanopia(coloredImages[imgIndex], header, imgsize, buffer, bitDepth, colorTable);

        simulate_cvd_deuteranopia(coloredImages[imgIndex], header, imgsize, buffer, bitDepth, colorTable);

        simulate_cvd_tritanopia(coloredImages[imgIndex], header, imgsize, buffer, bitDepth, colorTable);

        correct_cvd_protanopia(coloredImages[imgIndex], header, imgsize, buffer, bitDepth, colorTable);

        correct_cvd_deuteranopia(coloredImages[imgIndex], header, imgsize, buffer, bitDepth, colorTable);

        correct_cvd_tritanopia(coloredImages[imgIndex], header, imgsize, buffer, bitDepth, colorTable);

        black_and_white(coloredImages[imgIndex], header, imgsize, buffer, bitDepth, colorTable);

        image_bluring_color(coloredImages[imgIndex], header, imgsize, height, width, buffer, bitDepth, colorTable);

        image_rgb_rotate_right(coloredImages[imgIndex], header, height, width, D3buffer, colorTable);

        image_rgb_rotate_left(coloredImages[imgIndex], header, height, width, D3buffer, colorTable);

        image_rgb_rotate_180(coloredImages[imgIndex], header, height, width, D3buffer, colorTable);

        image_negative(coloredImages[imgIndex], header, height, width, D3buffer, colorTable);

        image_rgbtogray(coloredImages[imgIndex], header, height, width, D3buffer, colorTable);
    }

    MPI_Finalize();
    return 0;

}
