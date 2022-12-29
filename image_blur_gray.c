
#include <stdio.h>
#include <string.h>

int image_bluring_gray(char imageFileName[100])
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

	sprintf(ImageFilePath, "out/%s/image_bluring_gray.bmp", imageFileName);
	FILE *fOut = fopen(ImageFilePath, "w+"); // Output File name

	int j, y, x;

	fwrite(header, sizeof(unsigned char), 54, fOut); // write the header back

	if (bitDepth <= 8) // if ColorTable present, extract it.
	{
		fwrite(colorTable, sizeof(unsigned char), 1024, fOut);
	}

	unsigned char out[size]; // to store the image data
	memcpy(out, buffer, size);

	float v = 1.0 / 9.0;
	float kernel[3][3] = {{v, v, v}, // initialize the blurrring kernel
						  {v, v, v},
						  {v, v, v}};

	for (x = 1; x < height - 1; x++)
	{
		for (y = 1; y < width - 1; y++)
		{
			float sum = 0.0;
			for (i = -1; i <= 1; ++i)
			{
				for (j = -1; j <= 1; ++j)
				{
					sum = sum + (float)kernel[i + 1][j + 1] * buffer[(x + i) * width + (y + j)]; // matrix multiplication with kernel
				}
			}
			out[(x * width) + y] = sum;
		}
	}

	fwrite(out, sizeof(unsigned char), size, fOut); // write image data back to the file
	fclose(fOut);
	return 0;
}
