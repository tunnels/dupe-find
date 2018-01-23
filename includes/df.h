#include <stdlib.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// each component in each pixel will have a value from 0 to COMPONENT_MAX_VALUE
#define COMPONENT_MAX_VALUE 256.0

typedef struct image_struct_t {
	unsigned char *image_data;
	char *location;
	int width;
	int height;
	int number_of_pixels;
	int components_per_pixel;
	long *rgb_array; // store 'count' of each rgb component per pixel
} image_struct;

unsigned char *initializeImage(image_struct *image, int set_components_value);
void rgbComponentMethodStoreInArrays(image_struct *base, image_struct *comparison);
void rgbComponentMethodCalculatePercentages(image_struct *base, image_struct *comparison);
