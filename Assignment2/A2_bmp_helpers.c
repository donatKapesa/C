/* FILE: A2_bmp_helpers.c is where you will code your answers for Assignment 2.
 * 
 * Each of the functions below can be considered a start for you. 
 *
 * You should leave all of the code as is, except for what's surrounded
 * in comments like "REPLACE EVERTHING FROM HERE... TO HERE.
 *
 * The assignment document and the header A2_bmp_headers.h should help
 * to find out how to complete and test the functions. Good luck!
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

int max(unsigned int* a, unsigned int *b);
int fit(int x, int y, int x_max, int x_min, int y_max, int y_min);

// helper methods:

int max(unsigned int* a, unsigned int *b) {
	if (*a > *b) {
		return *a;
	} else
		return *b;
}

int fit(int x, int y, int x_max, int x_min, int y_max, int y_min) {
	if (x < x_max & x >= x_min & y < y_max & y >= y_min) {
		return 1;
	} else {
		return 0;
	}
}

int bmp_open(char* bmp_filename, unsigned int *width, unsigned int *height,
		unsigned int *bits_per_pixel, unsigned int *padding,
		unsigned int *data_size, unsigned int *data_offset,
		unsigned char** img_data) {

	// YOUR CODE FOR Q1 SHOULD REPLACE EVERYTHING FROM HERE

//  printf( "BMP_OPEN is not yet implemented. Please help complete this code!\n" );
//  *width=2;
//  *height=0;
//  *bits_per_pixel=0;
//  *padding=0;
//  *data_size=0;
//  *data_offset=0;
//  *img_data=NULL;

//Open a binary bmp file
	FILE *bmpfile = fopen(bmp_filename, "rb"); // add exception handling if it doesn't open

	// Read the B and M characters into chars
	char b, m;
	fread(&b, 1, 1, bmpfile);
	fread(&m, 1, 1, bmpfile);

	// Print the B and M to terminal
	printf("The first byte was: %c.\n", b);
	printf("The second byte was: %c.\n", m);

	// Read the overall file size
	//unsigned int *data_size;
	fread(data_size, 1, sizeof(unsigned int), bmpfile);
	printf("The size was: %d.\n", *data_size);

	// Close the file, re-open it to be at the beginning, and read the entire contents
	fclose(bmpfile);
	bmpfile = fopen(bmp_filename, "rb"); // add exception handling if it doesn't open

	*img_data = (unsigned char*) malloc(*data_size);
	fread(*img_data, 1, *data_size, bmpfile);
	//printf("\n size of file is: %lu\n", sizeof(img_data));
	fclose(bmpfile);

	/* MANAGED TO CLONE MARIO PIC HERE

	 FILE* new_bmpfile = fopen("new_mario.bmp", "wb");
	 fwrite(*img_data, 1, *data_size, new_bmpfile);
	 fclose(new_bmpfile);

	 */

	// getting data_offset
	unsigned int *offset = (unsigned int*) (*img_data + 10);
	*data_offset = *offset;

	// Read the width size into unsigned int (hope = 500 since this is the width of utah.bmp)
	unsigned int* wp = (unsigned int*) (*img_data + 18); // Question 2
	*width = *wp;

	// Read height into unsigned int
	unsigned int* hp = (unsigned int*) (*img_data + 22);
	*height = *hp;

	unsigned int* numOfBitsPerPixel = (unsigned int*) (*img_data + 28);
	*bits_per_pixel = *numOfBitsPerPixel;

	// Finding padding
	double x = (*width * *bits_per_pixel);
	double n = ceil(x / 32); // next multiple of four bytes that will store width*bbps bits
	*padding = (unsigned int) (((n * 32) - x) / 8);

	// Furaha padding formula: (4-((((canvas_width)*(bits_per_pixel1))/8)%4))%4

	// TO HERE

	return 0;
}

// We've implemented bmp_close for you. No need to modify this function
void bmp_close(unsigned char **img_data) {

	if (*img_data != NULL) {
		free(*img_data);
		*img_data = NULL;
	}
}

int bmp_mask(char* input_bmp_filename, char* output_bmp_filename,
		unsigned int x_min, unsigned int y_min, unsigned int x_max,
		unsigned int y_max, unsigned char red, unsigned char green,
		unsigned char blue) {
	unsigned int img_width;
	unsigned int img_height;
	unsigned int bits_per_pixel;
	unsigned int data_size;
	unsigned int padding;
	unsigned int data_offset;
	unsigned char* img_data = NULL;

	int open_return_code = bmp_open(input_bmp_filename, &img_width, &img_height,
			&bits_per_pixel, &padding, &data_size, &data_offset, &img_data);

	if (open_return_code) {
		printf(
				"bmp_open failed. Returning from bmp_mask without attempting changes.\n");
		return -1;
	}

	// YOUR CODE FOR Q2 SHOULD REPLACE EVERYTHING FROM HERE
	// allocating heap memory
	unsigned char* new_image_data = (unsigned char*) malloc(data_size);

	// copying old image contents to new image
	memcpy(new_image_data, img_data, data_size);

	// now setting colors
	unsigned int num_colors = bits_per_pixel / 8;
	unsigned char *pixel_data = new_image_data + data_offset; // change img_data to new_image_data
	for (unsigned int x = x_min; x <= x_max; x++) {
		for (unsigned int y = y_min; y <= y_max; y++) {
			pixel_data[y * (img_width * num_colors + padding) + x * num_colors
					+ 2] = blue;
			pixel_data[y * (img_width * num_colors + padding) + x * num_colors
					+ 1] = green;
			pixel_data[y * (img_width * num_colors + padding) + x * num_colors
					+ 0] = red;
		}
	}
	// copying new image array to new bmp file
	FILE* out_bmpfile = fopen(output_bmp_filename, "wb");
	fwrite(new_image_data, sizeof(unsigned int), data_size, out_bmpfile); // try 1 byte instead of unsigned int // Question 3
	fclose(out_bmpfile);

// TO HERE!

	bmp_close(&img_data);

	return 0;
}

int bmp_collage(char* bmp_input1, char* bmp_input2, char* bmp_result,
		int x_offset, int y_offset) {

	unsigned int img_width1;
	unsigned int img_height1;
	unsigned int bits_per_pixel1;
	unsigned int data_size1;
	unsigned int padding1;
	unsigned int data_offset1;
	unsigned char* img_data1 = NULL;

	int open_return_code = bmp_open(bmp_input1, &img_width1, &img_height1,
			&bits_per_pixel1, &padding1, &data_size1, &data_offset1,
			&img_data1);

	if (open_return_code) {
		printf(
				"bmp_open failed for %s. Returning from bmp_collage without attempting changes.\n",
				bmp_input1);
		return -1;
	}

	unsigned int img_width2;
	unsigned int img_height2;
	unsigned int bits_per_pixel2;
	unsigned int data_size2;
	unsigned int padding2;
	unsigned int data_offset2;
	unsigned char* img_data2 = NULL;

	open_return_code = bmp_open(bmp_input2, &img_width2, &img_height2,
			&bits_per_pixel2, &padding2, &data_size2, &data_offset2,
			&img_data2);

	if (open_return_code) {
		printf(
				"bmp_open failed for %s. Returning from bmp_collage without attempting changes.\n",
				bmp_input2);
		return -1;
	}

	// YOUR CODE FOR Q3 SHOULD REPLACE EVERYTHING FROM HERE
	printf(
			"BMP_COLLAGE is not yet implemented. Please help complete this code!\n");

	// testing max function
//	unsigned int a = 222;
//	unsigned int b = 18;
//
//	int c = max(&a, &b);

//	printf("\n\nTESTING MAX FUNCTION: %d\n\n", c); // works

	//data for bmp_result

	unsigned int img_width;
	unsigned int img_height;
	unsigned int bits_per_pixel;
	unsigned int data_size;
	unsigned int padding;
	unsigned int data_offset;
	unsigned char* img_data = NULL;

	// setting up dimensions for new image height and width
	if (y_offset < 0) {
		unsigned int temp1 = img_height1 - y_offset;
		img_height = max(&temp1, &img_height2);
	} else {
		unsigned int temp1 = img_height2 - y_offset;
		img_height = max(&img_height1, &temp1);

	}

	if (x_offset < 0) {
		unsigned int temp2 = img_width1 - x_offset;
		img_width = max(&temp2, &img_width2);
	} else {
		unsigned int temp2 = img_width2 + x_offset;
		img_width = max(&img_width1, &temp2);
	}

//	printf("\nmax height = %d", img_height);
//	printf("\nmax width = %d", img_width);

	// not sure if I'll need this
	unsigned int canvas_x_max = img_width;
	unsigned int canvas_y_max = img_height;

	//xy boundaries for img1
	unsigned int img1_x_min;
	unsigned int img1_y_min;
	unsigned int img1_x_max;
	unsigned int img1_y_max;

	//xy boundaries for img2
	unsigned int img2_x_min;
	unsigned int img2_y_min;
	unsigned int img2_x_max;
	unsigned int img2_y_max;

	//setting padding
	double x = (img_width * bits_per_pixel1);
	double n = ceil(x / 32); // next multiple of four bytes that will store width*bbps bits
	padding = (unsigned int) (((n * 32) - x) / 8); // padding = (4-((((img_width)*(bits_per_pixel))/8)%4))%4; // Furaha's method works same as mine

//	printf("\nMy padding for img1 is: %d and my padding for img_collage is: %d",
//			padding1, padding);

	// bytes for width and height
	unsigned int width_bytes = img_width * bits_per_pixel1; // mabye use BBP for img and not img1
	unsigned int height_bytes = img_height * bits_per_pixel1;
	unsigned int padding_bytes = padding / 8;

	unsigned int row_width = (width_bytes + padding_bytes) / 8; // in bytes

	// calculating data size
	data_size = (data_offset1)
			+ ((width_bytes + padding_bytes) * height_bytes / 8);

	// allocating heap memory to bmp_result
	img_data = (unsigned char*) calloc(data_size, sizeof(unsigned int)); // or unsigned char img_data[data_size];
	// img_data = (unsigned char*) malloc(data_size);

	// image size is total size - size of header
	unsigned int img_size = data_size - data_offset1;
	unsigned int num_colors;

	if (data_offset1 > data_offset2) {
		memcpy(img_data, img_data1, data_offset1);
		num_colors = bits_per_pixel1 / 8; // check this
	} else {
		memcpy(img_data, img_data2, data_offset2);
		num_colors = bits_per_pixel2 / 8;
	}

	// testing to see if I get the right B, M, data size, width height etc...
	unsigned char* test_B = img_data;
	unsigned char* test_M = img_data + 1;
	unsigned int* prev_data_size = (unsigned int*) (img_data + 2);
	unsigned int* prev_height = (unsigned int*) (img_data + 18);
	unsigned int* prev_width = (unsigned int*) (img_data + 22);
//	printf(
//			"\n\n\n======MY QUESTION 3 TESTS START HERE====== \nAt first bit we have: %c\n",
//			*test_B);
//	printf("\nAt second bit we have: %c", *test_M);
//	printf("\ndata size before modyfing is: %d", *prev_data_size);
//	printf("\nwidth before modyfing is: %d", *prev_height);
//	printf("\nheight before modyfing is: %d\n\n", *prev_width);

	// changing image1 data_size, width and height values to new img_data
	memcpy(img_data + 2, &data_size, sizeof(unsigned int));
	memcpy(img_data + 18, &img_width, sizeof(unsigned int));
	memcpy(img_data + 22, &img_height, sizeof(unsigned int));
	memcpy(img_data + 34, &img_size, sizeof(unsigned int));
//
//	printf("\n This is after modifying data size width and height\n");
//	printf("\n\ndata size after modyfing is: %d",
//			*((unsigned int*) (img_data + 2)) = data_size);
//	printf("\nimg1 width is: %d, img2 width is: %d and img_width is: %d",
//			img_width1, img_width2, *((unsigned int*) (img_data + 18))); // works
//	printf("\nimg1 height is: %d, img2 height is: %d and img_height is: %d",
//			img_height1, img_height2, *((unsigned int*) (img_data + 22))); // works
//	printf("\nMy data_offset1 is : %d and my data_offset is: %d", data_offset1,
//			*((unsigned int*) (img_data + 10)));
//	printf("\nMy BPP for img1 is: %d and my BPS for img_collage is: %d",
//			bits_per_pixel1, *((unsigned int*) (img_data + 28)));

	unsigned char* pixel_data = img_data + data_offset1;
	unsigned char* pixel_data1 = img_data1 + data_offset1;
	unsigned char* pixel_data2 = img_data2 + data_offset2;

	unsigned int num_colors1 = bits_per_pixel1 / 8;
	unsigned int num_colors2 = bits_per_pixel2 / 8;

	unsigned int red, blue, green;

	// getting x and y offsets for img1 and img2 to see where they meet
	unsigned int x_offset1, x_offset2, y_offset1, y_offset2;

	if (x_offset >= 0) {
		x_offset1 = 0;
		x_offset2 = x_offset;
	} else {
		x_offset2 = 0;
		x_offset1 = -x_offset;
	}
	if (y_offset >= 0) {
		y_offset1 = 0;
		y_offset2 = y_offset;
	} else {
		y_offset2 = 0;
		y_offset1 = -y_offset;
	}

	printf("\n");

	// img1 boundaries relative to its offset
	img1_x_min = x_offset1;
	img1_y_min = y_offset1;
	img1_x_max = img_width1 + x_offset1;
	img1_y_max = img_height1 + y_offset1;

	// img2 boundaries relative to its offset
	img2_x_min = x_offset2;
	img2_y_min = y_offset2;
	img2_x_max = img_width2 + x_offset2;
	img2_y_max = img_height2 + y_offset2;

	// copying pixels of image1 to img_collage

	for (int y = 0; y <= canvas_y_max; y++) {
		for (int x = 0; x <= canvas_x_max; x++) {
			if (fit(x, y, img2_x_max, img2_x_min, img2_y_max, img2_y_min)) {

				blue = pixel_data2[(y - y_offset2)
						* (img_width2 * num_colors2 + padding2)
						+ (x - x_offset2) * num_colors2 + 2];
				green = pixel_data2[(y - y_offset2)
						* (img_width2 * num_colors2 + padding2)
						+ (x - x_offset2) * num_colors2 + 1];
				red = pixel_data2[(y - y_offset2)
						* (img_width2 * num_colors2 + padding2)
						+ (x - x_offset2) * num_colors2 + 0];

			} else if (fit(x, y, img1_x_max, img1_x_min, img1_y_max,
					img1_y_min)) {

				blue = pixel_data1[(y - y_offset1)
						* (img_width1 * num_colors1 + padding1)
						+ (x - x_offset1) * num_colors1 + 2];
				green = pixel_data1[(y - y_offset1)
						* (img_width1 * num_colors1 + padding1)
						+ (x - x_offset1) * num_colors1 + 1];
				red = pixel_data1[(y - y_offset1)
						* (img_width1 * num_colors1 + padding1)
						+ (x - x_offset1) * num_colors1 + 0];

			} else {
				red = 255;
				green = 255;
				blue = 255;

			}
			// setting pixels in canvas
			pixel_data[y * (img_width * num_colors + padding) + x * num_colors
					+ 2] = blue;
			pixel_data[y * (img_width * num_colors + padding) + x * num_colors
					+ 1] = green;
			pixel_data[y * (img_width * num_colors + padding) + x * num_colors
					+ 0] = red;
		}
	}

	// writing to new file
	FILE* bmpfile = fopen(bmp_result, "wb");
	fwrite(img_data, sizeof(unsigned int), data_size, bmpfile);
	fclose(bmpfile);

	// TO HERE!

	bmp_close(&img_data1);
	bmp_close(&img_data2);

	return 0;
}
