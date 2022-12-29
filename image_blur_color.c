#include <stdio.h>
#include <string.h>

int image_bluring_color(char imageFileName[100])
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

	sprintf(ImageFilePath, "out/%s/bluring_color.bmp", imageFileName);
	FILE *fOut = fopen(ImageFilePath, "w+"); // Output File name

	int y, x;

	fwrite(header, sizeof(unsigned char), 54, fOut); // write the header back

	if (bitDepth <= 8) // if ColorTable present, extract it.
	{
		fwrite(colorTable, sizeof(unsigned char), 1024, fOut);
	}

	unsigned char out[size][3]; // store the output image data
	memcpy(out, buffer, (size * 3));

	float v = 1.0 / 9.0; // initialize the blurrring kernel
	float kernel[3][3] = {{v, v, v},
						  {v, v, v},
						  {v, v, v}};
	for (x = 1; x < height - 1; x++)
	{
		for (y = 1; y < width - 1; y++)
		{
			float sum0 = 0.0;
			float sum1 = 0.0;
			float sum2 = 0.0;
			for (i = -1; i <= 1; ++i)
			{
				for (j = -1; j <= 1; ++j)
				{
					// matrix multiplication with kernel with every color plane
					sum0 = sum0 + (float)kernel[i + 1][j + 1] * buffer[(x + i) * width + (y + j)][0];
					sum1 = sum1 + (float)kernel[i + 1][j + 1] * buffer[(x + i) * width + (y + j)][1];
					sum2 = sum2 + (float)kernel[i + 1][j + 1] * buffer[(x + i) * width + (y + j)][2];
				}
			}
			out[(x)*width + (y)][0] = sum0;
			out[(x)*width + (y)][1] = sum1;
			out[(x)*width + (y)][2] = sum2;
		}
	}

	for (i = 0; i < size; i++) // write image data back to the file
	{
		putc(out[i][2], fOut);
		putc(out[i][1], fOut);
		putc(out[i][0], fOut);
	}

	fclose(fOut);
	return 0;
}
