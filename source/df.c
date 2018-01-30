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

	image_struct *iterator = comparison_image;
	rgbComponentMethodStoreInArrays(base_image, comparison_image);
	
	while (iterator->next != NULL) {
		rgbComponentMethodStoreInArrays(base_image, iterator->next);
		iterator = iterator->next;
	}

	return 0;
}

bool verifyParams(int argc)
{
	if (argc != 3) {
		printf("please follow the following format\ndf [source_image] [directory_to_test]\n");
		return false;
	}

	return true;
}

void printImageDetails(image_struct *image)
{
	printf("\nlocation: %s\nw:%d, h:%d, c:%d\n", image->location, image->width, image->height, image->components_per_pixel);
}

void initializeImages(image_struct *base, image_struct *comparison)
{
	image_struct *iterator = comparison;
	base->next = NULL;
	comparison->next = NULL;
	image_struct *new_image = NULL;

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

			// if we're on the first image
			if (comparison->next == NULL) {
				new_image = comparison;
			}
			else {
				new_image = (image_struct *) malloc(sizeof(*iterator));
			}

			memset(full_path, 0, FILENAME_MAX * 2); // make sure to reset fullpath at each iteration
			
			// fullpath = directory_name + current_filename
			strncpy(full_path, comparison_directory_name, strlen(comparison_directory_name));
			strcat(full_path, directory_entry->d_name);

			strncpy(new_image->location, full_path, FILENAME_MAX);
			new_image->image_data = stbi_load(new_image->location, &new_image->width, &new_image->height, &new_image->components_per_pixel, 0);
			new_image->components_array = calloc(1, sizeof(long) * new_image->components_per_pixel);
			new_image->number_of_pixels = new_image->width * new_image->height;

			iterator->next = new_image;
			iterator = iterator->next;
		}
		closedir(directory_pointer);
	}
	else {
		perror("couldn't open the directory");
		return;
	}

	base->image_data = stbi_load(base->location, &base->width, &base->height, &base->components_per_pixel, 0);
	base->components_array = calloc(1, sizeof(long) * base->components_per_pixel);
	base->number_of_pixels = base->width * base->height;
}

void rgbComponentMethodStoreInArrays(image_struct *base, image_struct *comparison)
{
	if (base->width == comparison->width && base->height == comparison->height) {
		if (base->components_per_pixel == comparison->components_per_pixel) {
			for (int i = 0; i < (comparison->number_of_pixels * comparison->components_per_pixel); i++) {
				// there are only components_per_pixel_base elements in the rgb arrays
				base->components_array[i % base->components_per_pixel] += base->image_data[i];
				comparison->components_array[i % comparison->components_per_pixel] += comparison->image_data[i];
			}
		}
		else {
			printf("[unhandled]: images do not have equal components\n");
			return;
		}
	}
	else {
		printf("[unhandled]: image dimensions are different\n");
		return;
	}
	
	printf("number of reds [base]: %ld, greens: %ld, blues: %ld\n", base->components_array[0], base->components_array[1], base->components_array[2]);
	printf("number of reds [comparison]: %ld, greens: %ld, blues: %ld\n\n", comparison->components_array[0], comparison->components_array[1], comparison->components_array[2]);
}

void rgbComponentMethodCalculatePercentages(image_struct *base, image_struct *comparison)
{
	double denominator = comparison->width * comparison->height * comparison->components_per_pixel * COMPONENT_MAX_VALUE;

	for (int i = 0; i < comparison->components_per_pixel; i++) {
		double base_calculated_value = (base->components_array[i] / denominator) * 100;
		double comparison_calculated_value = (comparison->components_array[i] / denominator) * 100;

		printf("comonent %d: ", i);
		printf("[base] = %f  ", base_calculated_value);
		printf("[comparison] = %f  ", comparison_calculated_value);
		printf("[difference] = %f\n", (comparison_calculated_value - base_calculated_value));
	}
}
