/**
* @file image_rgbtogray.c
* @brief C program to convert an RGB image to grayscale.
* @author Priya Shah
* @version v1
* @date 2018-01-10
*/
#include <stdio.h>
#include <time.h>
#include <omp.h>

int main()
{
	double start, stop;

	// start = clock();
	start = omp_get_wtime() ;// Note the start time for profiling purposes.

	FILE *fIn = fopen("images/sample.bmp", "r");			// Input File name
	FILE *fOut = fopen("out_waleed/sample_gray.bmp", "w+");		// Output File name

	int i,j;
	unsigned char byte[54];
	
	if(fIn==NULL)												// check if the input file has not been opened succesfully.
	{											
		printf("File does not exist.\n");
	}

	for(i=0;i<54;i++)											//read the 54 byte header from fIn
	{									
		byte[i] = getc(fIn);								
	}

	fwrite(byte,sizeof(unsigned char),54,fOut);					//write the header back

	// // extract image height, width and bitDepth from imageHeader 
	int height = *(int*)&byte[18];
	int width = *(int*)&byte[22];
	int bitDepth = *(int*)&byte[28];

	printf("width: %d\n",width);
	printf("height: %d\n",height );

	int size = 2457600; // height * width; // calculate image size

	unsigned char buffer[size][3];
	printf("before defining y, size: %d\n", size);
	// to store the image data
	double y[2457600];									//to store the grayscale image data

	printf("after defining y, size: %d\n",size);

	for(i=0; i<size; i++){
		buffer[i][2] = getc(fIn); // blue
		buffer[i][1] = getc(fIn); // green
		buffer[i][0] = getc(fIn); // red
	}

	printf("after first array, size: %d\n", size);

	#pragma omp parallel for num_threads(16) private(y)
	for(i=0;i<size;i++)											//RGB to gray
	{
		y[i] =0;
		y[i] =(buffer[i][0]*0.3) + (buffer[i][1]*0.59)	+ (buffer[i][2]*0.11);
		// printf("after processing, size: %d\n", size);

		// #pragma omp critical
		// {
		// 	putc(y[i],fOut);
		// 	putc(y[i],fOut);
		// 	putc(y[i],fOut);
		// }		//conversion formula of rgb to gray
		putc(y[i], fOut);
		// printf("after first putc , size: %d\n", size);

		putc(y[i], fOut);
		putc(y[i], fOut);
	}

	// for(i=0;i<size;i++)											//write image data to output file
	// {
	// 	putc(y[i], fOut);
	// 	putc(y[i], fOut);
	// 	putc(y[i], fOut);
	// }


	fwrite(y,sizeof(unsigned char),size,fOut);			//write back to the output image

	
	fclose(fOut);
	fclose(fIn);

	// stop = clock();
	stop = omp_get_wtime();
	printf("\ntime = %f seconds\n",stop-start);


	// printf("\nCLOCKS_PER_SEC = %f\n",stop-start); 
	// printf("%lf ms\n",((double)(stop-start) * 1000.0)/CLOCKS_PER_SEC );
	return 0;
}
