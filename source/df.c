#include "../includes/df.h"

int main(int argc, char **argv)
{
	if (!verifyParams(argc, argv[1])) {
		return -1;
	}

	int number_of_images;
	Image *images[MAX_IMAGES_PER_DIR];
	initializeImages(&images[0], argv[1], &number_of_images);

	for (int i = 0; i < number_of_images; i++) {
		storeComponents(images[i]);
		// printComponents(images[i]);
	}

	for (int i = 0; i < number_of_images; i++) {
		int k = 0;

		// todo: [performance] ignore duplicate sets; if (image_1, image_2), ignore (image_2, image_1)
		for (int j = (i + 1) % number_of_images; (k + 1) < number_of_images; (j = (j + 1) % number_of_images, k++)) {
			compareComponents(images[i], images[j]);
		}
	}

	printf("finished [main]\n");
	return 0;
}

bool verifyParams(int argc, char *source_directory)
{
	if (argc != 2) {
		printf("please follow the following format\ndf [source_directory]\n");
		return false;
	}

	if (strlen(source_directory) >= FILENAME_MAX) {
		printf("[error] directory name is too long\n");
		return false;
	}

	printf("finished [verifyParams]\n");
	return true;
}

void printImageDetails(Image *image)
{
	printf("\nlocation: %s\nw:%d, h:%d, c:%d\n", image->location, image->width, image->height, image->components_per_pixel);
	printf("finished [printImageDetails]\n");
}

void initializeImages(Image *images[MAX_IMAGES_PER_DIR], char *source_directory, int *number_of_images)
{
	Image *new_image = NULL;

	// loop through all the images
	DIR *directory_pointer;
	struct dirent *directory_entry;
	char image_path[FILENAME_MAX * 2];

	directory_pointer = opendir(source_directory);
	if (directory_pointer != NULL) {
		while ((directory_entry = readdir(directory_pointer))) {
			// skip hidden files
			if (!strncmp(directory_entry->d_name, ".", 1)) {
				continue;
			}

			new_image = (Image *) malloc(sizeof(*new_image));

			// image_path = directory_name + current_filename
			memset(image_path, 0, FILENAME_MAX * 2);
			strncpy(image_path, source_directory, strlen(source_directory));
			strcat(image_path, directory_entry->d_name);
			strncpy(new_image->location, image_path, FILENAME_MAX * 2);

			new_image->image_data = stbi_load(new_image->location, &new_image->width, &new_image->height, &new_image->components_per_pixel, 0);
			new_image->components = calloc(1, sizeof(long) * new_image->components_per_pixel);
			new_image->number_of_pixels = new_image->width * new_image->height;

			if (*number_of_images < MAX_IMAGES_PER_DIR) {
				images[(*number_of_images)++] = new_image;
			}
			else {
				printf("[ERROR] too many images in directory\n");
				return;
			}
		}
		closedir(directory_pointer);
	}
	else {
		perror("ERROR");
	}

	printf("finished [initializeImages]\n");
}

void storeComponents(Image *image)
{
	for (int i = 0; i < (image->number_of_pixels * image->components_per_pixel); i++) {
		// reset this value for each component array entry
		if (i < image->components_per_pixel) {
			image->components[i] = 0;
		}

		// there are only components_per_pixel_base elements in the rgb arrays
		image->components[i % image->components_per_pixel] += image->image_data[i];
	}

	printf("finished [storeComponents]\n");
}

void printComponents(Image *image)
{
	for (int i = 0; i < image->components_per_pixel; i++)
	printf("component %d: %ld\n", i, image->components[i]);
	printf("finished [printComponents]\n");
}

void compareComponents(Image *base, Image *comparison)
{
	double base_denominator = base->width * base->height * base->components_per_pixel * COMPONENT_MAX_VALUE;
	double comparison_denominator = comparison->width * comparison->height * comparison->components_per_pixel * COMPONENT_MAX_VALUE;
	double total_difference = 0;

	printf("\ncomparing base: '%s' and comparison: '%s'\n", base->location, comparison->location);
	for (int i = 0; i < comparison->components_per_pixel; i++) {
		double base_calculated_value = (base->components[i] / base_denominator) * 100;
		double comparison_calculated_value = (comparison->components[i] / comparison_denominator) * 100;

		// printf("%d: ", i);
		// printf("[base] = %f ; ", base_calculated_value);
		// printf("[comparison] = %f ; ", comparison_calculated_value);
		total_difference += fabs(comparison_calculated_value - base_calculated_value);
		// printf("[difference] = %f\n", total_difference);
	}
	printf("[Total Diffirence] = %f\n", total_difference);
	// printf("comparing %s and %s\n", base->location, comparison->location);
	printf("finished [compareComponents]\n");
}
