#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "image_bright.c"
#include "image_dark.c"
#include "image_colortosepia.c"
#include "image_simulate_cvd.c"
#include "image_correct_cvd.c"
#include "image_rgbtogray.c"
#include "black_white.c"
#include "image_blur_color.c"
#include "image_blur_gray.c"
#include "image_rotate.c"
#include "negative_image.c"

<<<<<<< HEAD

int colored() {
=======
#ifdef RELEASE
#define printer
#else
#define printer printf
#endif

int colored(char imageFileName[])
{
	omp_set_num_threads(14);
>>>>>>> f16d40e49a5b884202995712d16eb0c88e64b490

	char ImageFilePath[150];
	sprintf(ImageFilePath, "images/%s.bmp", imageFileName);

	printer("******** This code is executing the colored image processing applications ***** \n");
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
<<<<<<< HEAD
	for(i=0;i<54;i++)						// read the 54 byte header from fIn
	{			
=======

#pragma omp parallel for num_threads(1) // ordered
	for (i = 0; i < 54; i++)			// read the 54 byte header from fIn
	{
>>>>>>> f16d40e49a5b884202995712d16eb0c88e64b490
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
<<<<<<< HEAD
	
	for(int i=0;i<width;i++)											
	{
		for (j=0;j<height;j++){
            D3buffer[i][j][2]=getc(fIn3D);									//blue
            D3buffer[i][j][1]=getc(fIn3D);									//green
            D3buffer[i][j][0]=getc(fIn3D);									//red
        }
=======

#pragma omp parallel for private(j) num_threads(1) //ordered
	for (int i = 0; i < width; i++)
	{
		for (j = 0; j < height; j++)
		{
			D3buffer[i][j][2] = getc(fIn3D); // blue
			D3buffer[i][j][1] = getc(fIn3D); // green
			D3buffer[i][j][0] = getc(fIn3D); // red
		}
>>>>>>> f16d40e49a5b884202995712d16eb0c88e64b490
	}
	unsigned char buffer[size][3]; // to store the image data

<<<<<<< HEAD
	for(i=0;i<size;i++){
=======
#pragma omp parallel for num_threads(1) //ordered 
	for (i = 0; i < size; i++)
	{
>>>>>>> f16d40e49a5b884202995712d16eb0c88e64b490
		{
			buffer[i][2] = getc(fIn); // blue
			buffer[i][1] = getc(fIn); // green
			buffer[i][0] = getc(fIn); // red
		}
	}
<<<<<<< HEAD
	printf("height: %d\n",height);
	printf("width: %d\n",width);
	printf("size: %d\n",size);
		image_colortosepia(header, size, buffer);

		simulate_cvd_protanopia(header, size, buffer);

		simulate_cvd_deuteranopia(header, size, buffer);

		simulate_cvd_tritanopia(header, size, buffer);

		correct_cvd_protanopia(header, size, buffer);

		correct_cvd_deuteranopia(header, size, buffer);

		correct_cvd_tritanopia(header, size, buffer);

		black_and_white(header, size, buffer, bitDepth, colorTable);

		image_bluring_color(header, size, height, width, buffer , bitDepth, colorTable);

		image_rgb_rotate_right(header, height, width, D3buffer, colorTable);

		image_rgb_rotate_left(header, height, width, D3buffer, colorTable);

		image_rgb_rotate_180(header, height, width, D3buffer, colorTable);

		image_negative(header, height, width, D3buffer, colorTable);

		image_rgbtogray(header, height, width, D3buffer, colorTable);

		fclose(fIn);
		return 0;
}


int nonColored() {

	printf("******** This code is executing the non-colored image processing applications ****** \n");
	FILE *fIn = fopen("images/lena512.bmp","r");			// Input File name
	unsigned char header[54];
	unsigned char colorTable[1024];
	int i;
    if(fIn==NULL)							// check if the input file has not been opened succesfully.
	{											
		printf("File does not exist.\n");
	}
	for(i=0;i<54;i++)						// read the 54 byte header from fIn
	{					
=======

	printer("height: %d\n", height);
	printer("width: %d\n", width);
	printer("size: %d\n", size);

#pragma omp parallel sections
	{
#pragma omp section
		image_colortosepia(imageFileName, header, size, buffer, bitDepth, colorTable);

#pragma omp section
		simulate_cvd_protanopia(imageFileName, header, size, buffer, bitDepth, colorTable);

#pragma omp section
		simulate_cvd_deuteranopia(imageFileName, header, size, buffer, bitDepth, colorTable);

#pragma omp section
		simulate_cvd_tritanopia(imageFileName, header, size, buffer, bitDepth, colorTable);

#pragma omp section
		correct_cvd_protanopia(imageFileName, header, size, buffer, bitDepth, colorTable);

#pragma omp section
		correct_cvd_deuteranopia(imageFileName, header, size, buffer, bitDepth, colorTable);

#pragma omp section
		correct_cvd_tritanopia(imageFileName, header, size, buffer, bitDepth, colorTable);

#pragma omp section
		black_and_white(imageFileName, header, size, buffer, bitDepth, colorTable);

#pragma omp section
		image_bluring_color(imageFileName, header, size, height, width, buffer, bitDepth, colorTable);

#pragma omp section
		image_rgb_rotate_right(imageFileName, header, height, width, D3buffer, colorTable);

#pragma omp section
		image_rgb_rotate_left(imageFileName, header, height, width, D3buffer, colorTable);

#pragma omp section
		image_rgb_rotate_180(imageFileName, header, height, width, D3buffer, colorTable);

#pragma omp section
		image_negative(imageFileName, header, height, width, D3buffer, colorTable);

#pragma omp section
		image_rgbtogray(imageFileName, header, height, width, D3buffer, colorTable);
	}

	fclose(fIn);
	return 0;
}

int nonColored(char imageFileName[])
{
	omp_set_num_threads(3);

	printer("******** This code is executing the non-colored image processing applications ****** \n");

	FILE *fIn = fopen(imageFileName, "r"); // Input File name
	unsigned char header[54];
	unsigned char colorTable[1024];
	int i;

	if (fIn == NULL) // check if the input file has not been opened succesfully.
	{
		printf("File does not exist.\n");
	}

#pragma omp parallel for num_threads(1) //ordered 
	for (i = 0; i < 54; i++) // read the 54 byte header from fIn
	{
>>>>>>> f16d40e49a5b884202995712d16eb0c88e64b490
		{
			header[i] = getc(fIn);
		}
	}
<<<<<<< HEAD
    int height = *(int*)&header[18];
	int width = *(int*)&header[22];
	int bitDepth = *(int*)&header[28];
	if(bitDepth<=8)										//if ColorTable present, extract it.
=======

	int height = *(int *)&header[18];
	int width = *(int *)&header[22];
	int bitDepth = *(int *)&header[28];

	if (bitDepth <= 8) // if ColorTable present, extract it.
>>>>>>> f16d40e49a5b884202995712d16eb0c88e64b490
	{
		fread(colorTable, sizeof(unsigned char), 1024, fIn);
	}

	int size = height * width;	// calculate image size
	unsigned char buffer[size]; // to store the image data

<<<<<<< HEAD
	for(i=0;i<size;i++){
		buffer[i] = getc(fIn);			
	}
	printf("height: %d\n",height);
	printf("width: %d\n",width);
	printf("size: %d\n",size);
	image_bluring_gray(header, size, height, width, buffer , bitDepth, colorTable);//lena512.bmp
	
    image_dark(header, colorTable, size, buffer);

    image_bright(header, colorTable, size, buffer);
   	fclose(fIn);
=======
#pragma omp parallel for num_threads(1) 
	for (i = 0; i < size; i++)
	{
		buffer[i] = getc(fIn);
	}

	printer("height: %d\n", height);
	printer("width: %d\n", width);
	printer("size: %d\n", size);

#pragma omp parallel sections
	{
#pragma omp section
		image_bluring_gray(header, size, height, width, buffer, bitDepth, colorTable); // lena512.bmp

#pragma omp section
		image_dark(header, colorTable, size, buffer);

#pragma omp section
		image_bright(header, colorTable, size, buffer);
	}
	fclose(fIn);
>>>>>>> f16d40e49a5b884202995712d16eb0c88e64b490
	return 0;
}

int C = 1;

int coloredImagesDriver()
{
	char coloredImages[6][100] = {
		"airplane",
		"baboon",
		"barbara",
		"BoatsColor",
		"goldhill",
		"lena_color",
	};

	for (int i = 0; i < 6; i++)
	{
		colored(coloredImages[i]);
	}

	return 0;
}

<<<<<<< HEAD
	clock_t CStart, CStop, NCStart, NCStop;

	CStart = clock();
	colored();
	CStop = clock();
	NCStart = clock();
	nonColored();
	NCStop = clock();

	printf("colored excution Time: %.2lf ms\n",((double)(CStop-CStart)/1000));
	printf("non-colored excution Time: %.2lf ms\n",((double)(NCStop-NCStart)/1000));
	printf("overall excution Time: %.2lf ms\n",((double)((NCStop+CStop)-(NCStart+CStart))/1000));
=======
int main(int argc, char *argv[])
{

	double CStart, CStop, NCStart, NCStop;

#pragma omp parallel num_threads(1)
	{
#pragma omp task
		{
			CStart = omp_get_wtime();
			coloredImagesDriver();
			CStop = omp_get_wtime();
		}
#pragma omp task
		{
			NCStart = omp_get_wtime();
			nonColored("images/boats.bmp");
			NCStop = omp_get_wtime();
		}
	}

	printf("colored excution Time: %lf ms\n", ((double)(CStop - CStart) * 1000));
	printf("non-colored excution Time: %lf ms\n", ((double)(NCStop - NCStart) * 1000));
	printf("overall excution Time: %lf ms\n", ((double)((NCStop + CStop) - (NCStart + CStart)) * 1000));
>>>>>>> f16d40e49a5b884202995712d16eb0c88e64b490
}