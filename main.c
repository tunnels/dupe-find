#include <stdlib.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define MAX_COLORS 256

int main()
{
	int width_a, height_a, components_per_pixel_a;
	unsigned char *base_image = stbi_load("images/a.png", &width_a, &height_a, &components_per_pixel_a, 0);
	int num_pixels_a = width_a * height_a;
	// int original_rgb_array[components_per_pixel_a][MAX_COLORS] = {0};
	int original_hex_array[components_per_pixel_a];
	memset(original_hex_array, 0, sizeof(int) * components_per_pixel_a);

	int width_b, height_b, components_per_pixel_b;
	unsigned char *comparison_image = stbi_load("images/b.png", &width_b, &height_b, &components_per_pixel_b, 0);
	int num_pixels_b = width_b * height_b;
	// int comparison_rgb_array[components_per_pixel_b][MAX_COLORS] = {0};
	int comparison_hex_array[components_per_pixel_b];
	memset(comparison_hex_array, 0, sizeof(int) * components_per_pixel_b);


	printf("image a\nw:%d, h:%d, c:%d\n", width_a, height_a, components_per_pixel_a);
	printf("\nimage b\nw:%d, h:%d, c:%d\n\n", width_b, height_b, components_per_pixel_b);

	int counter = 0;
	if (width_a == width_b && height_a == height_b) {
		for (int i = 0; i < (num_pixels_b * components_per_pixel_b); i += components_per_pixel_b) {
			// printf("RGB: %03d %03d %03d\t", *(base_image + i), *(base_image + (i + 1)), *(base_image + (i + 2)));
			// printf("RGB: %03d %03d %03d\n", *(comparison_image + i), *(comparison_image + (i + 1)), *(comparison_image + (i + 2)));

			// method 1
			// if (*(base_image + i) != *(comparison_image + i) || 
			// 	*(base_image + i + 1) != *(comparison_image + i + 1) || 
			// 	*(base_image + i + 2) != *(comparison_image + i + 2)) {
			// 	printf("not identical\n");
			// 	break;
			// }

			// method 2
			// printf("%d\n", *(base_image + i));
			original_hex_array[0] += *(base_image + i); original_hex_array[1] += *(base_image + i + 1); original_hex_array[2] += *(base_image + i + 2);
			comparison_hex_array[0] += *(comparison_image + i); comparison_hex_array[1] += *(comparison_image + i + 1); comparison_hex_array[2]+= *(comparison_image + i + 2);

			// if (i > 200) {
			// 	break;
			// }
		}
	}
	else {
		printf("not gonna try\n");
	}

	printf("number of reds: %d, greens: %d, blues: %d\n", original_hex_array[0], original_hex_array[1], original_hex_array[2]);
	printf("number of reds: %d, greens: %d, blues: %d\n\n", comparison_hex_array[0], comparison_hex_array[1], comparison_hex_array[2]);

	printf("\n\n");

	printf("percentage of reds: %f, greens: %f, blues: %f\n", 
		(original_hex_array[0]/(4*100*100*256.0))*100,
		(original_hex_array[1]/(4*100*100*256.0))*100,
		(original_hex_array[2]/(4*100*100*256.0))*100);
	printf("percentage of reds: %f, greens: %f, blues: %f\n\n", 
		(comparison_hex_array[0]/(4*100*100*256.0))*100, 
		(comparison_hex_array[1]/(4*100*100*256.0))*100, 
		(comparison_hex_array[2]/(4*100*100*256.0))*100);
	

	printf("\nthey seem identical\n");

	return 0;
}
