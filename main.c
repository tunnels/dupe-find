#include <stdlib.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// each component in each pixel will have a value from 0 to COMPONENT_MAX_VALUE
#define COMPONENT_MAX_VALUE 256 

unsigned char *initializeImage(char *image_location, int *width, int *height, int *components_per_pixel, int set_components_value);
void rgbComponentMethod(long *base_rgb_array, long *comparison_rgb_array, unsigned char *base_image, unsigned char *comparison_image, int width_base, int width_comparison, int height_base, int height_comparison, int num_pixels_comparison, int components_per_pixel_base, int components_per_pixel_comparison);

int main(void)
{
	// methods: checksum, rgb component array, tbc.

	int width_base, height_base, components_per_pixel_base;
	int width_comparison, height_comparison, components_per_pixel_comparison;

	char* base_image_location = "images/a.png";
	char* comparison_image_location = "images/b.png"; // todo: pass as argument

	unsigned char *base_image = initializeImage(base_image_location, &width_base, &height_base, &components_per_pixel_base, 0);
	unsigned char *comparison_image = initializeImage(comparison_image_location, &width_comparison, &height_comparison, &components_per_pixel_comparison, 0);

	// todo: check if images are loaded sucessfully

	int num_pixels_base = width_base * height_base;
	int num_pixels_comparison = width_comparison * height_comparison;

	// arrays to store 'count' of each rgb component per pixel
	long base_rgb_array[components_per_pixel_base];
	long comparison_rgb_array[components_per_pixel_comparison];
	memset(base_rgb_array, 0, sizeof(long) * components_per_pixel_base);
	memset(comparison_rgb_array, 0, sizeof(long) * components_per_pixel_comparison);

	printf("image a\nw:%d, h:%d, c:%d\n", width_base, height_base, components_per_pixel_base);
	printf("\nimage b\nw:%d, h:%d, c:%d\n\n", width_comparison, height_comparison, components_per_pixel_comparison);

	rgbComponentMethod(base_rgb_array, comparison_rgb_array, base_image, comparison_image, width_base, width_comparison,
		height_base, height_comparison, num_pixels_comparison, components_per_pixel_base, components_per_pixel_comparison);
	
	// if (checkPercentages() is under threshold) {
	// 	printf("\nthey seem identical\n");
	// }
	// else {
	// 	printf("\nnot identical\n");
	// }

	return 0;
}

unsigned char *initializeImage(char *image_location, int *width, int *height, int *components_per_pixel, int set_components_value)
{
	return stbi_load(image_location, width, height, components_per_pixel, set_components_value);
}

void rgbComponentMethod(long *base_rgb_array, long *comparison_rgb_array, unsigned char *base_image, unsigned char *comparison_image, int width_base, int width_comparison, int height_base, int height_comparison, int num_pixels_comparison, int components_per_pixel_base, int components_per_pixel_comparison)
{
	if (width_base == width_comparison && height_base == height_comparison) {
		if (components_per_pixel_base == components_per_pixel_comparison) {
			for (int i = 0; i < (num_pixels_comparison * components_per_pixel_comparison); i += components_per_pixel_comparison) {
				// method 1
				// if (*(base_image + i) != *(comparison_image + i) || 
				// 	*(base_image + i + 1) != *(comparison_image + i + 1) || 
				// 	*(base_image + i + 2) != *(comparison_image + i + 2)) {
				// 	printf("not identical\n");
				// 	break;
				// }

				// method 2
				// loop through componenents rather than assume there will be 3 (r,g,b) components + alpha
				base_rgb_array[0] += *(base_image + i); base_rgb_array[1] += *(base_image + i + 1); base_rgb_array[2] += *(base_image + i + 2);
				comparison_rgb_array[0] += *(comparison_image + i); comparison_rgb_array[1] += *(comparison_image + i + 1); comparison_rgb_array[2]+= *(comparison_image + i + 2);

				// lazy method
				// if (i > 200) {
				// 	break;
				// }
			}
		}
		else {
			printf("images do not have equal components\n");
			// todo: handle this
		}
	}
	else {
		printf("image dimensions are different\n");
		// todo: handle this
	}
	
	printf("number of reds [base]: %ld, greens: %ld, blues: %ld\n", base_rgb_array[0], base_rgb_array[1], base_rgb_array[2]);
	printf("number of reds [comparison]: %ld, greens: %ld, blues: %ld\n\n", comparison_rgb_array[0], comparison_rgb_array[1], comparison_rgb_array[2]);

	printf("percentage of reds [base]: %f, greens: %f, blues: %f\n", 
		(base_rgb_array[0]/(4*100*100*256.0))*100,
		(base_rgb_array[1]/(4*100*100*256.0))*100,
		(base_rgb_array[2]/(4*100*100*256.0))*100);
	printf("percentage of reds [comparison]: %f, greens: %f, blues: %f\n", 
		(comparison_rgb_array[0]/(4*100*100*256.0))*100, 
		(comparison_rgb_array[1]/(4*100*100*256.0))*100, 
		(comparison_rgb_array[2]/(4*100*100*256.0))*100);	
}
