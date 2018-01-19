#include <stdlib.h>
#include <stdio.h>

 #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main()
{
	int width_a, height_a, components_per_pixel_a;
	int width_b, height_b, components_per_pixel_b;

	unsigned char *base_image = stbi_load("images/a.png", &width_a, &height_a, &components_per_pixel_a, 0);
	unsigned char *comparison_image = stbi_load("images/b.png", &width_b, &height_b, &components_per_pixel_b, 0);

	int num_pixels_a = width_a * height_a;
	int num_pixels_b = width_a * height_a;

	printf("image a\nw: %d, h: %d, c: %d\n", width_a, height_a, components_per_pixel_a);
	// printf("RGBA: %d %d %d %d\n", *(base_image + 0), *(base_image + 1), *(base_image + 2), *(base_image + 3));

	// for (int i = 0; i < (num_pixels_a * components_per_pixel_a); i += components_per_pixel_a) {
	// 	printf("RGB: %d %d %d\n", *(base_image + i), *(base_image + (i + 1)), *(base_image + (i + 2)));

	// 	if (i > 30) {
	// 		break;
	// 	}
	// }

	printf("\nimage b\nw: %d, h: %d, c: %d\n", width_b, height_b, components_per_pixel_b);
	// for (int i = 0; i < (num_pixels_b * components_per_pixel_b); i += components_per_pixel_b) {
	// 	printf("RGB: %d %d %d\n", *(comparison_image + i), *(comparison_image + (i + 1)), *(comparison_image + (i + 2)));

	// 	if (i > 30) {
	// 		break;
	// 	}
	// }

	if (width_a == width_b && height_a == height_b) {
		for (int i = 0; i < (num_pixels_b * components_per_pixel_b); i += components_per_pixel_b) {
			// printf("RGB: %d %d %d\n", *(comparison_image + i), *(comparison_image + (i + 1)), *(comparison_image + (i + 2)));

			if (*(base_image + i) != *(comparison_image + i) || 
				*(base_image + i + 1) != *(comparison_image + i + 1) || 
				*(base_image + i + 2) != *(comparison_image + i + 2)) {
				printf("not identical\n");
				break;
			}

			if (i > 30) {
				break;
			}
		}
	}
	else {
		printf("not gonna try\n");
	}

	printf("\nthey seem identical\n");

	return 0;
}
