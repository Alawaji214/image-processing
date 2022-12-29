#include <stdio.h>
#define THRESHOLD 40

#ifdef RELEASE
#define printer
#else
#define printer printf
#endif

int image_dark(char imageFileName[100])
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

	sprintf(ImageFilePath, "out/%s/image_dark.bmp", imageFileName);
	FILE *fOut = fopen(ImageFilePath, "w+"); // Output File name

	fwrite(header, sizeof(unsigned char), 54, fOut); // write the header back

	// extract image height, width and bitDepth from imageHeader

	if (bitDepth <= 8) // if ColorTable present, extract it.
	{
		fwrite(colorTable, sizeof(unsigned char), 1024, fOut);
	}

	unsigned char image_buffer[size];
	// Parallelizable segment
	for (i = 0; i < size; i++) // decreasing pixel values to get image bright
	{
		if (buffer[i] > THRESHOLD)
			image_buffer[i] = buffer[i] - THRESHOLD;
		else
			image_buffer[i] = 0;
		// image_buffer[i] = buffer[i];
	}

	fwrite(image_buffer, sizeof(unsigned char), size, fOut); // write back to the output image
	fclose(fOut);

	return 0;
}
