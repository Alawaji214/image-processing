#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int image_rgb_rotate_right(char imageFileName[100])
{
	char ImageFilePath[150];
	sprintf(ImageFilePath, "images/%s.bmp", imageFileName);
	printer(" ==  %s \n", ImageFilePath);

	FILE *fIn = fopen(ImageFilePath, "r");	 // Input File name
	FILE *fIn3D = fopen(ImageFilePath, "r"); // Input File name

	unsigned char header[54];
	unsigned char colorTable[1024];
	int i, j;

	if (fIn == NULL) // check if the input file has not been opened succesfully.
	{
		printf("File does not exist.\n");
	}

	for (i = 0; i < 54; i++) // read the 54 byte header from fIn
	{
		{
			header[i] = getc(fIn);
			getc(fIn3D);
		}
	}

	int height = *(int *)&header[18];
	int width = *(int *)&header[22];
	int bitDepth = *(int *)&header[28];
	if (bitDepth <= 8) // if ColorTable present, extract it.
	{
		fread(colorTable, sizeof(unsigned char), 1024, fIn);
	}

	int size = height * width;				  // calculate image size
	unsigned char D3buffer[width][height][3]; // to store the image data

	for (int i = 0; i < width; i++)
	{
		for (j = 0; j < height; j++)
		{
			D3buffer[i][j][2] = getc(fIn3D); // blue
			D3buffer[i][j][1] = getc(fIn3D); // green
			D3buffer[i][j][0] = getc(fIn3D); // red
		}
	}
	unsigned char buffer[size][3]; // to store the image data

	for (i = 0; i < size; i++)
	{
		{
			buffer[i][2] = getc(fIn); // blue
			buffer[i][1] = getc(fIn); // green
			buffer[i][0] = getc(fIn); // red
		}
	}

	fclose(fIn3D);
	fclose(fIn);

	sprintf(ImageFilePath, "out/%s/rotate_right.bmp", imageFileName);
	FILE *fOut = fopen(ImageFilePath, "w+"); // Output File name

	int tempH, tempW;
	unsigned char heightA[4];
	unsigned char widthA[4];

	unsigned char out_buffer[height][width][3];
	unsigned char new_header[54];
	memcpy(new_header, header, 54);

	for (i = 0; i < width; i++) // to rotate right
	{
		tempH = height;
		for (j = 0; j < height; j++)
		{
			tempH--;
			out_buffer[tempH][i][2] = D3buffer[i][j][2];
			out_buffer[tempH][i][1] = D3buffer[i][j][1];
			out_buffer[tempH][i][0] = D3buffer[i][j][0];
		}
	}

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

int image_rgb_rotate_left(char imageFileName[100])
{
	char ImageFilePath[150];
	sprintf(ImageFilePath, "images/%s.bmp", imageFileName);
	printer(" ==  %s \n", ImageFilePath);

	FILE *fIn = fopen(ImageFilePath, "r");	 // Input File name
	FILE *fIn3D = fopen(ImageFilePath, "r"); // Input File name

	unsigned char header[54];
	unsigned char colorTable[1024];
	int i, j;

	if (fIn == NULL) // check if the input file has not been opened succesfully.
	{
		printf("File does not exist.\n");
	}

	for (i = 0; i < 54; i++) // read the 54 byte header from fIn
	{
		{
			header[i] = getc(fIn);
			getc(fIn3D);
		}
	}

	int height = *(int *)&header[18];
	int width = *(int *)&header[22];
	int bitDepth = *(int *)&header[28];
	if (bitDepth <= 8) // if ColorTable present, extract it.
	{
		fread(colorTable, sizeof(unsigned char), 1024, fIn);
	}

	int size = height * width;				  // calculate image size
	unsigned char D3buffer[width][height][3]; // to store the image data

	for (int i = 0; i < width; i++)
	{
		for (j = 0; j < height; j++)
		{
			D3buffer[i][j][2] = getc(fIn3D); // blue
			D3buffer[i][j][1] = getc(fIn3D); // green
			D3buffer[i][j][0] = getc(fIn3D); // red
		}
	}
	unsigned char buffer[size][3]; // to store the image data

	for (i = 0; i < size; i++)
	{
		{
			buffer[i][2] = getc(fIn); // blue
			buffer[i][1] = getc(fIn); // green
			buffer[i][0] = getc(fIn); // red
		}
	}

	fclose(fIn3D);
	fclose(fIn);

	sprintf(ImageFilePath, "out/%s/rotate_left.bmp", imageFileName);
	FILE *fOut = fopen(ImageFilePath, "w+"); // Output File name

	int choice;
	int tempW;
	unsigned char heightA[4];
	unsigned char widthA[4];

	unsigned char out_buffer[height][width][3];
	unsigned char new_header[54];
	memcpy(new_header, header, 54);

	for (i = 0; i < height; i++) // to rotate left
	{
		tempW = width;
		for (j = 0; j < width; j++)
		{
			tempW--;
			out_buffer[i][tempW][2] = D3buffer[j][i][2];
			out_buffer[i][tempW][1] = D3buffer[j][i][1];
			out_buffer[i][tempW][0] = D3buffer[j][i][0];
		}
	}

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

int image_rgb_rotate_180(char imageFileName[100])
{
	char ImageFilePath[150];
	sprintf(ImageFilePath, "images/%s.bmp", imageFileName);
	printer(" ==  %s \n", ImageFilePath);

	FILE *fIn = fopen(ImageFilePath, "r");	 // Input File name
	FILE *fIn3D = fopen(ImageFilePath, "r"); // Input File name

	unsigned char header[54];
	unsigned char colorTable[1024];
	int i, j;

	if (fIn == NULL) // check if the input file has not been opened succesfully.
	{
		printf("File does not exist.\n");
	}

	for (i = 0; i < 54; i++) // read the 54 byte header from fIn
	{
		{
			header[i] = getc(fIn);
			getc(fIn3D);
		}
	}

	int height = *(int *)&header[18];
	int width = *(int *)&header[22];
	int bitDepth = *(int *)&header[28];
	if (bitDepth <= 8) // if ColorTable present, extract it.
	{
		fread(colorTable, sizeof(unsigned char), 1024, fIn);
	}

	int size = height * width;				  // calculate image size
	unsigned char D3buffer[width][height][3]; // to store the image data

	for (int i = 0; i < width; i++)
	{
		for (j = 0; j < height; j++)
		{
			D3buffer[i][j][2] = getc(fIn3D); // blue
			D3buffer[i][j][1] = getc(fIn3D); // green
			D3buffer[i][j][0] = getc(fIn3D); // red
		}
	}
	unsigned char buffer[size][3]; // to store the image data

	for (i = 0; i < size; i++)
	{
		{
			buffer[i][2] = getc(fIn); // blue
			buffer[i][1] = getc(fIn); // green
			buffer[i][0] = getc(fIn); // red
		}
	}

	fclose(fIn3D);
	fclose(fIn);

	sprintf(ImageFilePath, "out/%s/rotate_180.bmp", imageFileName);
	FILE *fOut = fopen(ImageFilePath, "w+"); // Output File name

	int choice;
	unsigned char heightA[4];
	unsigned char widthA[4];

	unsigned char out_buffer[width][height][3];

	for (i = width - 1; i >= 0; i--) // to rotate left
	{
		for (j = height - 1; j >= 0; j--)
		{
			out_buffer[i][j][2] = D3buffer[width - 1 - i][j][2];
			out_buffer[i][j][1] = D3buffer[width - 1 - i][j][1];
			out_buffer[i][j][0] = D3buffer[width - 1 - i][j][0];
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
