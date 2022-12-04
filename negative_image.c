#include <stdio.h>
#include <stdlib.h>
#include <time.h>

<<<<<<< HEAD
int image_negative(unsigned char header[54], int height, int width, unsigned char buffer[width][height][3], unsigned char colorTable[1024])
=======
int image_negative(char imageFileName[100], unsigned char header[54], int height, int width, unsigned char buffer[width][height][3], unsigned char colorTable[1024])
>>>>>>> f16d40e49a5b884202995712d16eb0c88e64b490
{
	char ImageFilePath[150];
	sprintf(ImageFilePath, "out/%s/negative_image.bmp", imageFileName);
	FILE *fOut = fopen(ImageFilePath, "w+"); // Output File name

	int i, j; // loop counter variables
	unsigned char out_buffer[width][height][3];
	int bitDepth = *(int *)&header[28];

<<<<<<< HEAD
	//Calculate the mean of the image
	for(i = 0; i < width; i++){
	      for(j = 0; j < height; j++){
			  out_buffer[i][j][0] = 0xFF - buffer [i][j][0];
			  out_buffer[i][j][1] = 0xFF - buffer [i][j][1];
			  out_buffer[i][j][2] = 0xFF - buffer [i][j][2];
	      }
=======
// Calculate the mean of the image
	for (i = 0; i < width; i++)
	{
		for (j = 0; j < height; j++)
		{
			out_buffer[i][j][0] = 0xFF - buffer[i][j][0];
			out_buffer[i][j][1] = 0xFF - buffer[i][j][1];
			out_buffer[i][j][2] = 0xFF - buffer[i][j][2];
		}
>>>>>>> f16d40e49a5b884202995712d16eb0c88e64b490
	}

	fwrite(header, sizeof(unsigned char), 54, fOut); // write the header back
	if (bitDepth <= 8)								 // if ColorTable present, extract it.
	{
		fwrite(colorTable, sizeof(unsigned char), 1024, fOut);
	}

<<<<<<< HEAD
=======
#pragma omp parallel for num_threads(1)
>>>>>>> f16d40e49a5b884202995712d16eb0c88e64b490
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
