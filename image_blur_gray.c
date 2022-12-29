#include <stdio.h>
#include <string.h>

int image_bluring_gray(char imageFileName[100], unsigned char header[54], int size, int height, int width, unsigned char *buffer, int bitDepth, unsigned char colorTable[1024])
{
	// Create the output file path
	char ImageFilePath[150];
	sprintf(ImageFilePath, "out/%s/image_bluring_gray.bmp", imageFileName);
	FILE *fOut = fopen(ImageFilePath, "w+"); // Output File name

	// Allocate memory for the output image data
	unsigned char *out = malloc(size * sizeof(unsigned char));
	if (out == NULL)
	{
		fprintf(stderr, "Error allocating memory for out array\n");
		return 1;
	}

	// Initialize the blurrring kernel
	float v = 1.0 / 9.0;
	float kernel[3][3] = {{v, v, v},
						  {v, v, v},
						  {v, v, v}};

	// Perform the blurrring operation
	for (int x = 1; x < height - 1; x++)
	{
		for (int y = 1; y < width - 1; y++)
		{
			float sum = 0.0;
			for (int i = -1; i <= 1; ++i)
			{
				for (int j = -1; j <= 1; ++j)
				{
					sum = sum + (float)kernel[i + 1][j + 1] * buffer[((x + i) * width + (y + j))];
				}
			}
			out[(x * width) + y] = sum;
		}
	}

	// Write the output image data to the file
	fwrite(header, sizeof(unsigned char), 54, fOut); // write the header back
	if (bitDepth <= 8)
	{ // if ColorTable present, extract it.
		fwrite(colorTable, sizeof(unsigned char), 1024, fOut);
	}
	fwrite(out, sizeof(unsigned char), size, fOut); // write image data back to the file

	// Clean up
	fclose(fOut);
	free(out);
	return 0;
}
