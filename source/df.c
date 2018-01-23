#include "../includes/df.h"

int main(void)
{
	/*
	todo:
		methods: checksum, rgb component array, tbc.
		add support for multiple threads
	*/

	image_struct *base_image = malloc(sizeof(image_struct));
	image_struct *comparison_image = malloc(sizeof(image_struct));

	base_image->location = "tests/a.png";
	comparison_image->location = "tests/b.png"; // todo: pass as argument

	base_image->image_data = initializeImage(base_image, 0);
	comparison_image->image_data = initializeImage(comparison_image, 0);

	// todo: check if images are loaded sucessfully

	base_image->number_of_pixels = base_image->width * base_image->height;
	comparison_image->number_of_pixels = comparison_image->width * comparison_image->height;

	base_image->rgb_array = calloc(1, sizeof(long) * base_image->components_per_pixel);
	comparison_image->rgb_array = calloc(1, sizeof(long) * comparison_image->components_per_pixel);

	printf("image a\nw:%d, h:%d, c:%d\n", base_image->width, base_image->height, base_image->components_per_pixel);
	printf("\nimage b\nw:%d, h:%d, c:%d\n\n", comparison_image->width, comparison_image->height, comparison_image->components_per_pixel);

	rgbComponentMethodStoreInArrays(base_image, comparison_image);
	rgbComponentMethodCalculatePercentages(base_image, comparison_image);

	// todo: implement the method presented in pseudocode below
	// if (*(base_image + i) != *(comparison_image + i) || 
	// 	*(base_image + i + 1) != *(comparison_image + i + 1) || 
	// 	*(base_image + i + 2) != *(comparison_image + i + 2)) {
	// 	printf("not identical\n");
	// 	break;
	// }

	// if (checkPercentages() is under threshold) {
	// 	printf("\nthey seem identical\n");
	// }
	// else {
	// 	printf("\nnot identical\n");
	// }

	return 0;
}

unsigned char *initializeImage(image_struct *image, int set_components_value)
{
	return stbi_load(image->location, &image->width, &image->height, &image->components_per_pixel, set_components_value);
}

void rgbComponentMethodStoreInArrays(image_struct *base, image_struct *comparison)
{
	// skip adjacent pixels or only get 1 out of every 10 pixels or something, requires testing for (configurable) sweet spot
	// rgbComponentMethodBuildShrinkPixelsToCheck();
	if (base->width == comparison->width && base->height == comparison->height) {
		if (base->components_per_pixel == comparison->components_per_pixel) {
			for (int i = 0; i < (comparison->number_of_pixels * comparison->components_per_pixel); i++) {
				// there are only components_per_pixel_base elements in the rgb arrays
				base->rgb_array[i % base->components_per_pixel] += base->image_data[i];
				comparison->rgb_array[i % comparison->components_per_pixel] += comparison->image_data[i];
			}
		}
		else {
			printf("images do not have equal components\n");
			// todo: handle this
		}
	}
	else {
		printf("image dimensions are different\n\n");
		// todo: handle this
	}
	
	printf("number of reds [base]: %ld, greens: %ld, blues: %ld\n", base->rgb_array[0], base->rgb_array[1], base->rgb_array[2]);
	printf("number of reds [comparison]: %ld, greens: %ld, blues: %ld\n\n", comparison->rgb_array[0], comparison->rgb_array[1], comparison->rgb_array[2]);
}

void rgbComponentMethodCalculatePercentages(image_struct *base, image_struct *comparison)
// void rgbComponentMethodCalculatePercentages(long *base_rgb_array, long *comparison_rgb_array, int width_comparison, int height_comparison, int components_per_pixel_comparison)
{
	double denominator = comparison->width * comparison->height * comparison->components_per_pixel * COMPONENT_MAX_VALUE;

	for (int i = 0; i < comparison->components_per_pixel; i++) {
		double base_calculated_value = (base->rgb_array[i] / denominator) * 100;
		double comparison_calculated_value = (comparison->rgb_array[i] / denominator) * 100;

		printf("comonent %d: ", i);
		printf("[base] = %f  ", base_calculated_value);
		printf("[comparison] = %f  ", comparison_calculated_value);
		printf("[difference] = %f\n", (comparison_calculated_value - base_calculated_value));
	}
}
