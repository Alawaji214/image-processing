#include <stdio.h>
#include <time.h>
#define BW_THRESHOLD 128
#define WHITE 255
#define BLACK 0

int black_and_white(char imageFileName[100])
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

	sprintf(ImageFilePath, "out/%s/black_and_white.bmp", imageFileName);
	FILE *fOut = fopen(ImageFilePath, "w+"); // Output File name

	int y;

	fwrite(header, sizeof(unsigned char), 54, fOut); // write the header back

	if (bitDepth <= 8) // if ColorTable present, extract it.
	{
		fwrite(colorTable, sizeof(unsigned char), 1024, fOut);
	}

	for (i = 0; i < size; i++) // store 0(black) and 255(white) values to buffer
	{
		if ((buffer[i][0] > BW_THRESHOLD && buffer[i][1] > BW_THRESHOLD) || (buffer[i][0] > BW_THRESHOLD && buffer[i][2] > BW_THRESHOLD) || (buffer[i][1] > BW_THRESHOLD && buffer[i][2] > BW_THRESHOLD))
		{
			y = WHITE;
		}
		else
		{
			y = BLACK;
		}
		{
			putc(y, fOut);
			putc(y, fOut);
			putc(y, fOut);
		}
	}

	fclose(fOut);

	return 0;
}
