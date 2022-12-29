#include <stdio.h>
#include <stdlib.h>
#define MAX_COLOR 255
#define BRIGHTNESS_FACTOR 25

#ifdef RELEASE
#define printer
#else
#define printer printf
#endif


int image_bright(char imageFileName[100])
{
	char ImageFilePath[150];
	sprintf(ImageFilePath, "images/%s.bmp", imageFileName);
	printer(" ==  %s \n", ImageFilePath);
	FILE *fIn = fopen(ImageFilePath, "r"); // Input File name

	unsigned char header[54];
	unsigned char colorTable[1024];
	int i;

	if (fIn == NULL) // check if the input file has not been opened succesfully.
	{
		printf("File does not exist.\n");
	}

	for (i = 0; i < 54; i++) // read the 54 byte header from fIn
	{
		{
			header[i] = getc(fIn);
		}
	}

	int height = *(int *)&header[18];
	int width = *(int *)&header[22];
	int bitDepth = *(int *)&header[28];

	if (bitDepth <= 8) // if ColorTable present, extract it.
	{
		fread(colorTable, sizeof(unsigned char), 1024, fIn);
	}

	int size = height * width;	// calculate image size
	unsigned char buffer[size]; // to store the image data

	for (i = 0; i < size; i++)
	{
		buffer[i] = getc(fIn);
	}

	sprintf(ImageFilePath, "out/%s/image_bright.bmp", imageFileName);
	FILE *fOut = fopen(ImageFilePath, "w+"); // Output File name

	fwrite(header, sizeof(unsigned char), 54, fOut); // write the header back

	int temp;
	if (bitDepth <= 8) // if ColorTable present, extract it.
	{
		fwrite(colorTable, sizeof(unsigned char), 1024, fOut);
	}
	unsigned char image_buffer[size];

	for (i = 0; i < size; i++) // increasing pixel values to get image bright
	{
		temp = buffer[i] + BRIGHTNESS_FACTOR;
		image_buffer[i] = (temp > MAX_COLOR) ? MAX_COLOR : temp;
	}

	fwrite(image_buffer, sizeof(unsigned char), size, fOut); // write back to the output image

	fclose(fOut);

	return 0;
}
