#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// each component in each pixel will have a value from 1 to COMPONENT_MAX_VALUE
#define COMPONENT_MAX_VALUE 256.0

typedef struct image_struct_t {
	unsigned char *image_data;
	char location[FILENAME_MAX];
	int width;
	int height;
	int number_of_pixels;
	int components_per_pixel;
	long *components_array;
	struct image_struct_t *next;
} image_struct;

bool verifyParams(int argc);
void printImageDetails(image_struct *image);
void initializeImages(image_struct *base, image_struct *comparison);
void componentMethodStoreInArrays(image_struct *base, image_struct *comparison);
void printComponentArray(image_struct *base);
void comparePercentageDifferenceBetweenComponentArrays(image_struct *base, image_struct *comparison);
