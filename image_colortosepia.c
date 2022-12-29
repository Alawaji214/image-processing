#include <stdio.h>
#include <time.h>
#define MAX_VALUE 255

#ifdef RELEASE
#define printer
#else
#define printer printf
#endif

int image_colortosepia(char imageFileName[100])
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

	sprintf(ImageFilePath, "out/%s/sepia.bmp", imageFileName);
	FILE *fOut = fopen(ImageFilePath, "w+"); // Output File name

	int r, g, b;

	fwrite(header, sizeof(unsigned char), 54, fOut); // write the header back

	for (i = 0; i < size; i++)
	{
		r = 0;
		g = 0;
		b = 0;

		// conversion formula of rgb to sepiabuffer
		r = (buffer[i][2] * 0.393) + (buffer[i][1] * 0.769) + (buffer[i][0] * 0.189);
		g = (buffer[i][2] * 0.349) + (buffer[i][1] * 0.686) + (buffer[i][0] * 0.168);
		b = (buffer[i][2] * 0.272) + (buffer[i][1] * 0.534) + (buffer[i][0] * 0.131);

		if (r > MAX_VALUE)
		{ // if value exceeds
			r = MAX_VALUE;
		}
		if (g > MAX_VALUE)
		{
			g = MAX_VALUE;
		}
		if (b > MAX_VALUE)
		{
			b = MAX_VALUE;
		}
		putc(b, fOut);
		putc(g, fOut);
		putc(r, fOut);
	}
	fclose(fOut);

	return 0;
}
