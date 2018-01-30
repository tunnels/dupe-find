#include "../includes/df.h"

int main(int argc, char *argv[])
{
	image_struct *base_image = malloc(sizeof(image_struct));
	image_struct *comparison_image = malloc(sizeof(image_struct));

	if (!verifyParams(argc)) {
		return -1;
	}

	strncpy(base_image->location, argv[1], FILENAME_MAX);
	strncpy(comparison_image->location, argv[2], FILENAME_MAX);

	initializeImages(base_image, comparison_image);

	base_image->number_of_pixels = base_image->width * base_image->height;
	comparison_image->number_of_pixels = comparison_image->width * comparison_image->height;

	base_image->rgb_array = calloc(1, sizeof(long) * base_image->components_per_pixel);
	comparison_image->rgb_array = calloc(1, sizeof(long) * comparison_image->components_per_pixel);

	printf("image a\nw:%d, h:%d, c:%d\n", base_image->width, base_image->height, base_image->components_per_pixel);
	printf("\nimage b\nw:%d, h:%d, c:%d\n\n", comparison_image->width, comparison_image->height, comparison_image->components_per_pixel);

	rgbComponentMethodStoreInArrays(base_image, comparison_image);
	rgbComponentMethodCalculatePercentages(base_image, comparison_image);

	return 0;
}

bool verifyParams(int argc)
{
	if (argc != 3) {
		printf("please follow the following format\ndf [source_image] [directory_to_test] -[args]\n");
		return false;
	}

	return true;
}

void initializeImages(image_struct *base, image_struct *comparison)
{
	image_struct *iterator = comparison;
	base->next_image = NULL;
	comparison->next_image = NULL;

	char comparison_directory_name[FILENAME_MAX];
	strncpy(comparison_directory_name, comparison->location, FILENAME_MAX);

	// loop through all the comparison images
	DIR *directory_pointer;
	struct dirent *directory_entry;
	char full_path[FILENAME_MAX * 2];

	directory_pointer = opendir(comparison_directory_name);
	if (directory_pointer != NULL) {
		while ((directory_entry = readdir(directory_pointer))) {
			// skip hidden files
			if (!strncmp(directory_entry->d_name, ".", 1)) {
				continue;
			}

			image_struct *new_image = (image_struct *) malloc(sizeof(*iterator));
			memset(full_path, 0, FILENAME_MAX * 2); // make sure to reset fullpath at each iteration
			
			// fullpath = directory_name + current_filename
			strncpy(full_path, comparison_directory_name, strlen(comparison_directory_name));
			strcat(full_path, directory_entry->d_name);

			strncpy(new_image->location, full_path, FILENAME_MAX);
			new_image->image_data = stbi_load(new_image->location, &new_image->width, &new_image->height, &new_image->components_per_pixel, 0);

			iterator->next_image = new_image;
			iterator = iterator->next_image;
		}
		closedir(directory_pointer);
	}
	else {
		perror("couldn't open the directory");
		return;
	}
	base->image_data = stbi_load(base->location, &base->width, &base->height, &base->components_per_pixel, 0);
}

void rgbComponentMethodStoreInArrays(image_struct *base, image_struct *comparison)
{
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
		}
	}
	else {
		printf("image dimensions are different\n\n");
	}
	
	printf("number of reds [base]: %ld, greens: %ld, blues: %ld\n", base->rgb_array[0], base->rgb_array[1], base->rgb_array[2]);
	printf("number of reds [comparison]: %ld, greens: %ld, blues: %ld\n\n", comparison->rgb_array[0], comparison->rgb_array[1], comparison->rgb_array[2]);
}

void rgbComponentMethodCalculatePercentages(image_struct *base, image_struct *comparison)
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
