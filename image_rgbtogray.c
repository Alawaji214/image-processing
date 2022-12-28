#include <stdio.h>
#include <time.h>

int image_rgbtogray(char imageFileName[100], unsigned char header[54], int height, int width, unsigned char *buffer, unsigned char colorTable[1024])
{
	char ImageFilePath[150];
	sprintf(ImageFilePath, "out/%s/image_gray.bmp", imageFileName);

	FILE *fOut = fopen(ImageFilePath, "w+"); // Output File name

	int bitDepth = *(int *)&header[28];
	unsigned char out_buffer[height][width];

	fwrite(header, sizeof(unsigned char), 54, fOut); // write the header back
	if (bitDepth <= 8)								 // if ColorTable present, extract it.
	{
		fwrite(colorTable, sizeof(unsigned char), 1024, fOut);
	}

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			// Convert pixel from RGB to grayscale using formula:
			// grayscale = 0.3 * red + 0.59 * green + 0.11 * blue
			out_buffer[y][x] = (0.3 * buffer[(y * width + x) * 3 + 0]) + (0.59 * buffer[(y * width + x) * 3 + 1]) + (0.11 * buffer[(y * width + x) * 3 + 2]);
		}
	}

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			// Write grayscale value to all three color channels (R, G, B)
			putc(out_buffer[y][x], fOut);
			putc(out_buffer[y][x], fOut);
			putc(out_buffer[y][x], fOut);
		}
	}
	fclose(fOut);
	return 0;
}