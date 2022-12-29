#include <stdio.h>
#include <time.h>
#include "color_blindess.c"

#ifdef RELEASE
#define printer
#else
#define printer printf
#endif

int simulate_cvd_protanopia(char imageFileName[100])
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

	sprintf(ImageFilePath, "out/%s/simulate_protanopia.bmp", imageFileName);
	FILE *fOut = fopen(ImageFilePath, "w+"); // Output File name

	if (fOut == NULL) // check if the input file has not been opened succesfully.
	{
		printf("File did not open.\n");
	}

	int y, x;
	float l, m, s; // original
	unsigned char r, g, b;
	float ll, mm, ss; // updated

	fwrite(header, sizeof(unsigned char), 54, fOut); // write the header back
	unsigned char out[size][3];						 // store the output image data

	for (i = 0; i < size; i++)
	{
		b = buffer[i][0]; // blue
		g = buffer[i][1]; // green
		r = buffer[i][2]; // red

		rgb2lms(r, g, b, &l, &m, &s);

		simulate_protanopia(l, m, s, &ll, &mm, &ss);

		lms2rgb(ll, mm, ss, &r, &g, &b);

		out[i][0] = r;
		out[i][1] = g;
		out[i][2] = b;
	}

	for (i = 0; i < size; i++)
	{
		putc(out[i][2], fOut);
		putc(out[i][1], fOut);
		putc(out[i][0], fOut);
	}

	fclose(fOut);
	return 0;
}
int simulate_cvd_deuteranopia(char imageFileName[100])
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

	sprintf(ImageFilePath, "out/%s/simulate_deuteranopia.bmp", imageFileName);
	FILE *fOut = fopen(ImageFilePath, "w+"); // Output File name

	int y, x;
	float l, m, s; // original
	unsigned char r, g, b;
	float ll, mm, ss; // updated

	fwrite(header, sizeof(unsigned char), 54, fOut); // write the header back
	unsigned char out[size][3];						 // store the output image data

	for (i = 0; i < size; i++)
	{
		b = buffer[i][0]; // blue
		g = buffer[i][1]; // green
		r = buffer[i][2]; // red

		rgb2lms(r, g, b, &l, &m, &s);

		simulate_deuteranopia(l, m, s, &ll, &mm, &ss);

		lms2rgb(ll, mm, ss, &r, &g, &b);

		out[i][0] = r;
		out[i][1] = g;
		out[i][2] = b;
	}

	for (i = 0; i < size; i++)
	{
		putc(out[i][2], fOut);
		putc(out[i][1], fOut);
		putc(out[i][0], fOut);
	}

	fclose(fOut);
	return 0;
}
int simulate_cvd_tritanopia(char imageFileName[100])
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

	sprintf(ImageFilePath, "out/%s/simulate_tritanopia.bmp", imageFileName);
	FILE *fOut = fopen(ImageFilePath, "w+"); // Output File name

	int y, x;
	float l, m, s; // original
	unsigned char r, g, b;
	float ll, mm, ss; // updated

	fwrite(header, sizeof(unsigned char), 54, fOut); // write the header back
	unsigned char out[size][3];						 // store the output image data

	for (i = 0; i < size; i++)
	{
		b = buffer[i][0]; // blue
		g = buffer[i][1]; // green
		r = buffer[i][2]; // red

		rgb2lms(r, g, b, &l, &m, &s);

		simulate_tritanopia(l, m, s, &ll, &mm, &ss);

		lms2rgb(ll, mm, ss, &r, &g, &b);

		out[i][0] = r;
		out[i][1] = g;
		out[i][2] = b;
	}

	for (i = 0; i < size; i++)
	{
		putc(out[i][2], fOut);
		putc(out[i][1], fOut);
		putc(out[i][0], fOut);
	}

	fclose(fOut);

	return 0;
}
