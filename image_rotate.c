#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

<<<<<<< HEAD
int image_rgb_rotate_right(unsigned char header[54], int height, int width, unsigned char buffer[width][height][3], unsigned char colorTable[1024])
=======
int image_rgb_rotate_right(char imageFileName[100], unsigned char header[54], int height, int width, unsigned char buffer[width][height][3], unsigned char colorTable[1024])
>>>>>>> f16d40e49a5b884202995712d16eb0c88e64b490
{
	char ImageFilePath[150];
	sprintf(ImageFilePath, "out/%s/rotate_right.bmp", imageFileName);
	FILE *fOut = fopen(ImageFilePath, "w+"); // Output File name

	int i, j;
	int tempH, tempW;
	unsigned char heightA[4];
	unsigned char widthA[4];
	int bitDepth = *(int *)&header[28];
	int size = height * width; // calculate image size

	unsigned char out_buffer[height][width][3];
	unsigned char new_header[54];
	memcpy(new_header, header, 54);

<<<<<<< HEAD
	for (i = 0; i < width; i++) // to rotate right
=======
#pragma omp parallel for private(j, tempH) num_threads(1)
	for (i = 0; i < width; i++)			   // to rotate right
>>>>>>> f16d40e49a5b884202995712d16eb0c88e64b490
	{
		tempH = height;
		for (j = 0; j < height; j++)
		{
			tempH--;
			out_buffer[tempH][i][2] = buffer[i][j][2];
			out_buffer[tempH][i][1] = buffer[i][j][1];
			out_buffer[tempH][i][0] = buffer[i][j][0];
		}
	}

<<<<<<< HEAD
	for (i=0;i<4;i++){
		heightA[i]=header[18+i];
		widthA[i]=header[22+i];
		header[18 + i] = widthA[i];
		header[22 + i] = heightA[i];
=======
#pragma omp parallel for // num_threads(threads)
	for (i = 0; i < 4; i++)
	{
		heightA[i] = header[18 + i];
		widthA[i] = header[22 + i];
		new_header[18 + i] = widthA[i];
		new_header[22 + i] = heightA[i];
>>>>>>> f16d40e49a5b884202995712d16eb0c88e64b490
	}

	fwrite(new_header, sizeof(unsigned char), 54, fOut); // write the header back
	if (bitDepth <= 8)									 // if ColorTable present, extract it.
	{
		fwrite(colorTable, sizeof(unsigned char), 1024, fOut);
	}

<<<<<<< HEAD
=======
#pragma omp parallel for num_threads(1)
>>>>>>> f16d40e49a5b884202995712d16eb0c88e64b490
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			putc(out_buffer[i][j][2], fOut);
			putc(out_buffer[i][j][1], fOut);
			putc(out_buffer[i][j][0], fOut);
		}
	}
	fclose(fOut);
	return 0;
}

<<<<<<< HEAD
int image_rgb_rotate_left(unsigned char header[54], int height, int width, unsigned char buffer[width][height][3], unsigned char colorTable[1024])
=======
int image_rgb_rotate_left(char imageFileName[100], unsigned char header[54], int height, int width, unsigned char buffer[width][height][3], unsigned char colorTable[1024])
>>>>>>> f16d40e49a5b884202995712d16eb0c88e64b490
{
	char ImageFilePath[150];
	sprintf(ImageFilePath, "out/%s/rotate_left.bmp", imageFileName);
	FILE *fOut = fopen(ImageFilePath, "w+"); // Output File name

	int i, j, choice;
	int tempW;
	unsigned char heightA[4];
	unsigned char widthA[4];
	int bitDepth = *(int *)&header[28];
	int size = height * width; // calculate image size

	unsigned char out_buffer[height][width][3];
	unsigned char new_header[54];
	memcpy(new_header, header, 54);

<<<<<<< HEAD
	for (i = 0; i < height; i++) // to rotate left
=======
#pragma omp parallel for private(j, tempW) // num_threads(threads)
	for (i = 0; i < height; i++)		   // to rotate left
>>>>>>> f16d40e49a5b884202995712d16eb0c88e64b490
	{
		tempW = width;
		for (j = 0; j < width; j++)
		{
			tempW--;
			out_buffer[i][tempW][2] = buffer[j][i][2];
			out_buffer[i][tempW][1] = buffer[j][i][1];
			out_buffer[i][tempW][0] = buffer[j][i][0];
		}
	}

<<<<<<< HEAD
=======
#pragma omp parallel for // num_threads(threads)
>>>>>>> f16d40e49a5b884202995712d16eb0c88e64b490
	for (i = 0; i < 4; i++)
	{
		heightA[i] = header[18 + i];
		widthA[i] = header[22 + i];
		new_header[18 + i] = widthA[i];
		new_header[22 + i] = heightA[i];
	}
	fwrite(new_header, sizeof(unsigned char), 54, fOut); // write the header back
	if (bitDepth <= 8)									 // if ColorTable present, extract it.
	{
		fwrite(colorTable, sizeof(unsigned char), 1024, fOut);
	}

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			putc(out_buffer[i][j][2], fOut);
			putc(out_buffer[i][j][1], fOut);
			putc(out_buffer[i][j][0], fOut);
		}
	}
	fclose(fOut);
	return 0;
}

<<<<<<< HEAD
int image_rgb_rotate_180(unsigned char header[54], int height, int width, unsigned char buffer[width][height][3], unsigned char colorTable[1024])
=======
int image_rgb_rotate_180(char imageFileName[100], unsigned char header[54], int height, int width, unsigned char buffer[width][height][3], unsigned char colorTable[1024])
>>>>>>> f16d40e49a5b884202995712d16eb0c88e64b490
{

	char ImageFilePath[150];
	sprintf(ImageFilePath, "out/%s/rotate_180.bmp", imageFileName);
	FILE *fOut = fopen(ImageFilePath, "w+"); // Output File name

	int i, j, choice;
	unsigned char heightA[4];
	unsigned char widthA[4];

	int bitDepth = *(int *)&header[28];

	int size = height * width; // calculate image size
	unsigned char out_buffer[width][height][3];
<<<<<<< HEAD
	
	for (i = width-1; i >= 0; i--) // to rotate left
=======

#pragma omp parallel for private(j) num_threads(1)
	for (i = width - 1; i >= 0; i--)	   // to rotate left
>>>>>>> f16d40e49a5b884202995712d16eb0c88e64b490
	{
		for (j = height - 1; j >= 0; j--)
		{
			out_buffer[i][j][2] = buffer[width - 1 - i][j][2];
			out_buffer[i][j][1] = buffer[width - 1 - i][j][1];
			out_buffer[i][j][0] = buffer[width - 1 - i][j][0];
		}
	}

	fwrite(header, sizeof(unsigned char), 54, fOut); // write the header back
	if (bitDepth <= 8)								 // if ColorTable present, extract it.
	{
		fwrite(colorTable, sizeof(unsigned char), 1024, fOut);
	}

	for (i = 0; i < width; i++)
	{
		for (j = 0; j < height; j++)
		{
			putc(out_buffer[i][j][2], fOut);
			putc(out_buffer[i][j][1], fOut);
			putc(out_buffer[i][j][0], fOut);
		}
	}
	fclose(fOut);
	return 0;
}
