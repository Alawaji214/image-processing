
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "image_bright.c"
#include "image_dark.c"
#include "image_colortosepia.c"
#include "image_simulate_cvd.c" //there is an issue related to this piece of code #To be reviewed 
#include "image_correct_cvd.c"
#include "image_rgbtogray.c"
#include "black_white.c"
#include "image_blur_color.c"
#include "image_blur_gray.c"
#include "image_rotate.c"
#include "negative_image.c"


int colored() {

	printf("******** This code is executing the colored image processing applications ****** \n");
	FILE *fIn = fopen("images/airplane.bmp","r");			// Input File name
	FILE *fIn3D = fopen("images/airplane.bmp", "r");		// Input File name
	unsigned char header[54];
	unsigned char colorTable[1024];
	int i, j;
	if(fIn==NULL)							// check if the input file has not been opened succesfully.
	{											
		printf("File does not exist.\n");
	}
	for(i=0;i<54;i++)						// read the 54 byte header from fIn
	{			
		{
			header[i] = getc(fIn);
			getc(fIn3D);
		}												
	}
	
    int height = *(int*)&header[18];
	int width = *(int*)&header[22];
	int bitDepth = *(int*)&header[28];
	if (bitDepth <= 8) // if ColorTable present, extract it.
	{
		fread(colorTable, sizeof(unsigned char), 1024, fIn);
	}

	int size = height*width;		//calculate image size
	unsigned char D3buffer[width][height][3]; // to store the image data
	
	for(int i=0;i<width;i++)											
	{
		for (j=0;j<height;j++){
            D3buffer[i][j][2]=getc(fIn3D);									//blue
            D3buffer[i][j][1]=getc(fIn3D);									//green
            D3buffer[i][j][0]=getc(fIn3D);									//red
        }
	}
	unsigned char buffer[size][3];								//to store the image data

	for(i=0;i<size;i++){
		{
			buffer[i][2] = getc(fIn); // blue
			buffer[i][1] = getc(fIn); // green
			buffer[i][0] = getc(fIn); // red
		}
	}
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
		{
			header[i] = getc(fIn);
		}			
	}
    int height = *(int*)&header[18];
	int width = *(int*)&header[22];
	int bitDepth = *(int*)&header[28];
	if(bitDepth<=8)										//if ColorTable present, extract it.
	{
		fread(colorTable,sizeof(unsigned char),1024,fIn);
	}

	int size = height*width;	//calculate image size					
	unsigned char buffer[size];	//to store the image data

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
	return 0;
}


int C = 1;

int main(int argc, char *argv[]) {

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
}