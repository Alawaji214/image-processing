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

void colored(int rank, int size, MPI_Comm comm)
{

    int numImages = 24;
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

    for (int imgIndex = 0; imgIndex < numImages; imgIndex++)
    {
        if (rank == 0)
        {
            sprintf(ImageFilePath, "images/%s.bmp", coloredImages[imgIndex]);
            fIn = fopen(ImageFilePath, "r");   // Input File name
            fIn3D = fopen(ImageFilePath, "r"); // Input File name
            for (int i = 0; i < 54; i++)       // read the 54 byte header from fIn
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
                    D3buffer[(i * width + j) * 3 + 2] = getc(fIn3D); // blue
                    D3buffer[(i * width + j) * 3 + 1] = getc(fIn3D); // green
                    D3buffer[(i * width + j) * 3 + 0] = getc(fIn3D); // red
                }
            }
            buffer = malloc(imgsize * 3); // to store the image data

            for (int i = 0; i < imgsize; i++)
            {
                {
                    buffer[i * 3 + 2] = getc(fIn); // blue
                    buffer[i * 3 + 1] = getc(fIn); // green
                    buffer[i * 3 + 0] = getc(fIn); // red
                }
            }
        }

        MPI_Bcast(&height, 1, MPI_INT, 0, comm);
        MPI_Bcast(&width, 1, MPI_INT, 0, comm);
        MPI_Bcast(&bitDepth, 1, MPI_INT, 0, comm);
        MPI_Bcast(&imgsize, 1, MPI_INT, 0, comm);

        if (rank != 0)
        {
            D3buffer = malloc(width * height * 3);
            buffer = malloc(imgsize * 3);
        }
        MPI_Bcast(buffer, imgsize * 3, MPI_UNSIGNED_CHAR, 0, comm);
        MPI_Bcast(D3buffer, imgsize * 3, MPI_UNSIGNED_CHAR, 0, comm);

        MPI_Bcast(&header, 54, MPI_UNSIGNED_CHAR, 0, comm);
        MPI_Bcast(&colorTable, 1024, MPI_UNSIGNED_CHAR, 0, comm);
        // }

        int indices_per_process = 14 / size;
        int remaining_indices = 14 % size;
        int function_indices_for_process[indices_per_process + 1];

        for (int i = 0; i < indices_per_process; i++)
        {
            function_indices_for_process[i] = -1;
        }
        if (rank == 0)
            if (size < 15)
            {
                int function_indices[14] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

                MPI_Scatter(function_indices, indices_per_process + (rank < remaining_indices), MPI_INT, function_indices_for_process, indices_per_process + (rank < remaining_indices), MPI_INT, 0, MPI_COMM_WORLD);
            }
            else{
            int function_indices[size];
            for(int i = 0; i < size; i++){
                if(i < 14)
                    function_indices[i] = i;
                else
                    function_indices[i] = -1;
            }

            MPI_Scatter(function_indices, indices_per_process + (rank < remaining_indices), MPI_INT, function_indices_for_process, indices_per_process + (rank < remaining_indices), MPI_INT, 0, MPI_COMM_WORLD);
        }

        for (int i = 0; i < indices_per_process + 1; i++)
        {
            if(function_indices_for_process[i] < 0 || function_indices_for_process[i] > 13)
            {
                break;
            }
            switch (function_indices_for_process[i])
            {
            case 0:
                image_colortosepia(coloredImages[imgIndex], header, imgsize, buffer, bitDepth, colorTable);
                break;
            case 1:
                simulate_cvd_protanopia(coloredImages[imgIndex], header, imgsize, buffer, bitDepth, colorTable);
                break;
            case 2:
                simulate_cvd_deuteranopia(coloredImages[imgIndex], header, imgsize, buffer, bitDepth, colorTable);
                break;
            case 3:
                simulate_cvd_tritanopia(coloredImages[imgIndex], header, imgsize, buffer, bitDepth, colorTable);
                break;
            case 4:
                correct_cvd_protanopia(coloredImages[imgIndex], header, imgsize, buffer, bitDepth, colorTable);
                break;
            case 5:
                correct_cvd_deuteranopia(coloredImages[imgIndex], header, imgsize, buffer, bitDepth, colorTable);
                break;
            case 6:
                correct_cvd_tritanopia(coloredImages[imgIndex], header, imgsize, buffer, bitDepth, colorTable);
                break;
            case 7:
                black_and_white(coloredImages[imgIndex], header, imgsize, buffer, bitDepth, colorTable);
                break;
            case 8:
                image_bluring_color(coloredImages[imgIndex], header, imgsize, height, width, buffer, bitDepth, colorTable);
                break;
            case 9:
                image_rgb_rotate_right(coloredImages[imgIndex], header, height, width, D3buffer, colorTable);
                break;
            case 10:
                image_rgb_rotate_left(coloredImages[imgIndex], header, height, width, D3buffer, colorTable);
                break;
            case 11:
                image_rgb_rotate_180(coloredImages[imgIndex], header, height, width, D3buffer, colorTable);
                break;
            case 12:
                image_negative(coloredImages[imgIndex], header, height, width, D3buffer, colorTable);
                break;
            case 13:
                image_rgbtogray(coloredImages[imgIndex], header, height, width, D3buffer, colorTable);
                break;
            default:
                break;
            }
        }
    }
}

void nonColored(int rank, int size, MPI_Comm comm)
{

    char nonColoredImages[24][100] = {
        "boats",
        "boats",
        "boats",
        "boats",
        "boats",
        "boats",
        "boats",
        "boats",
        "boats",
        "boats",
        "boats",
        "boats",
        "boats",
        "boats",
        "boats",
        "boats",
        "boats",
        "boats",
        "boats",
        "boats",
        "boats",
        "boats",
        "boats",
        "boats",
    };

    int function_index;
    int numImages_nc = 24;
    char ImageFilePath[150];
    FILE *fIn;
    unsigned char header[54];
    unsigned char colorTable[1024];
    int height;
    int width;
    int bitDepth;
    int imgsize;
    unsigned char *buffer;

    for (int imgIndex = 0; imgIndex < numImages_nc; imgIndex++)
    {
        if (rank == 0)
        {
            sprintf(ImageFilePath, "images/%s.bmp", nonColoredImages[imgIndex]);
            fIn = fopen(ImageFilePath, "r"); // Input File name
            for (int i = 0; i < 54; i++)     // read the 54 byte header from fIn
            {
                {
                    header[i] = getc(fIn);
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
            buffer = malloc(imgsize);
            // to store the image data

            for (int i = 0; i < imgsize; i++)
            {
                buffer[i * sizeof(unsigned char)] = getc(fIn);
            }
        }

        MPI_Bcast(&height, 1, MPI_INT, 0, comm);
        MPI_Bcast(&width, 1, MPI_INT, 0, comm);
        MPI_Bcast(&bitDepth, 1, MPI_INT, 0, comm);
        MPI_Bcast(&imgsize, 1, MPI_INT, 0, comm);

        if (rank != 0)
        {
            buffer = malloc(imgsize);
        }

        MPI_Bcast(buffer, imgsize, MPI_UNSIGNED_CHAR, 0, comm);
        MPI_Bcast(&header, 54, MPI_UNSIGNED_CHAR, 0, comm);
        MPI_Bcast(&colorTable, 1024, MPI_UNSIGNED_CHAR, 0, comm);

        int indices_per_process = 3 / size;
        int remaining_indices = 3 % size;
        int function_indices_for_process[indices_per_process + 1];

        for (int i = 0; i < indices_per_process; i++)
        {
            function_indices_for_process[i] = -1;
        }

        if(rank==0)
        if (size < 3)
        {
            int function_indices[3] = {0, 1, 2};

            MPI_Scatter(function_indices, indices_per_process + (rank < remaining_indices), MPI_INT, function_indices_for_process, indices_per_process + (rank < remaining_indices), MPI_INT, 0, MPI_COMM_WORLD);
        }
        else
        {
            int function_indices[size];
            for (int i = 0; i < size; i++)
            {
                if (i < 3)
                    function_indices[i] = i;
                else
                    function_indices[i] = -1;
            }

            MPI_Scatter(function_indices, indices_per_process + (rank < remaining_indices), MPI_INT, function_indices_for_process, indices_per_process + (rank < remaining_indices), MPI_INT, 0, MPI_COMM_WORLD);
        }




        for (int i = 0; i < indices_per_process + 1; i++)
        {
            if (function_indices_for_process[i] < 0 || function_indices_for_process[i] > 2)
            {
                break;
            }

            switch (function_indices_for_process[i])
            {
            case 0:
                image_bluring_gray(nonColoredImages[imgIndex], header, imgsize, height, width, buffer, bitDepth, colorTable);
                break;
            case 1:
                image_dark(nonColoredImages[imgIndex], header, imgsize, buffer, bitDepth, colorTable);
                break;
            case 2:
                image_bright(nonColoredImages[imgIndex], header, imgsize, buffer, bitDepth, colorTable);
                break;
            default:
                break;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    // Initialize the MPI environment

    MPI_Init(&argc, &argv);
    MPI_Comm comm = MPI_COMM_WORLD;
    int rank, size;
    clock_t start, end;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if(rank == 0){
        start = clock();
    }


    colored(rank, size, comm);
    
    MPI_Barrier(MPI_COMM_WORLD);

    nonColored(rank, size, comm);

    MPI_Barrier(MPI_COMM_WORLD);

    if(rank == 0){
        end = clock();
        double time_taken = ((double)end - (double)start) / CLOCKS_PER_SEC;
        printf("number of Nodes = %d \t Time taken: %f\n",size, time_taken);
    }
    MPI_Finalize();
    return 0;
}
