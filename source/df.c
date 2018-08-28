#include "../includes/df.h"

int main(int argc, char **argv)
{
	if (!verifyParams(argc, argv[1])) {
		return -1;
	}

	int number_of_images;
	Image *images[MAX_IMAGES_PER_DIR];
	initializeImages(&images[0], argv[1], &number_of_images);

	// for (int i = 0; i < number_of_images; i++) {
	// 	storeChannels(images[i]);
	// 	// printChannels(images[i]);
	// 	// printImageDetails(images[i]);
	// }

	for (int i = 0; i < number_of_images; i++) {
		for (int j = i + 1; j < number_of_images; j++) {
			compareChannels(images[i], images[j]);
		}
		free(images[i]);
	}

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

	return true;
}

void printImageDetails(Image *image)
{
	printf("\nlocation: %s\nw:%d, h:%d, c:%d\n", image->location, image->width, image->height, image->channels_per_pixel);
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

			new_image->image_data = stbi_load(new_image->location, &new_image->width, &new_image->height, &new_image->channels_per_pixel, 0);
			new_image->channels = calloc(sizeof(long long), new_image->channels_per_pixel);
			new_image->number_of_pixels = new_image->width * new_image->height;

			storeChannels(new_image);
			free(new_image->image_data);

			if (*number_of_images < MAX_IMAGES_PER_DIR) {
				images[(*number_of_images)++] = new_image;
			}
			else {
				printf("[ERROR] too many images in directory\n");
				exit(-1);
			}
		}
		closedir(directory_pointer);
	}
	else {
		perror("ERROR");
	}
}

void storeChannels(Image *image)
{
	for (int i = 0; i < (image->number_of_pixels * image->channels_per_pixel); i++) {
		// reset this value for each channel array entry
		if (i < image->channels_per_pixel) {
			image->channels[i] = 0;
		}

		// there are only channels_per_pixel_base elements in the rgb arrays
		image->channels[i % image->channels_per_pixel] += image->image_data[i];
	}
}

void printChannels(Image *image)
{
	for (int i = 0; i < image->channels_per_pixel; i++) {
		printf("channel %d: %lld\n", i, image->channels[i]);
	}
	printf("\n");
}

void compareChannels(Image *base, Image *comparison)
{
	int channels_to_consider = (base->channels_per_pixel <= comparison->channels_per_pixel) ? base->channels_per_pixel : comparison->channels_per_pixel;

	double base_denominator = base->width * base->height * channels_to_consider * CHANNEL_MAX_VALUE;
	double comparison_denominator = comparison->width * comparison->height * channels_to_consider * CHANNEL_MAX_VALUE;

	double base_calculated_value = 0;
	double comparison_calculated_value = 0;
	double total_difference = 0;
	
	for (int i = 0; i < channels_to_consider; i++) {
		base_calculated_value = (base->channels[i] / base_denominator) * 100;
		comparison_calculated_value = (comparison->channels[i] / comparison_denominator) * 100;
		total_difference += fabs(comparison_calculated_value - base_calculated_value);

		// printf("%d: ", i);
		// printf("difference = %f (", total_difference);
		// printf("base = %f, ", base_calculated_value);
		// printf("comparison = %f)\n", comparison_calculated_value);
	}
	analyzeChannelDifference(total_difference, base->location, comparison->location);
}

void analyzeChannelDifference(double total_difference, char *base_location, char *comparison_location)
{
	if (total_difference == 0) {
		// printf("\n%s and %s\n", base_location, comparison_location);
		// printf("\t[PERFECT DUPLICATES]\n");
	}
	else if (total_difference <= 0.01) {
		printf("\n%s and %s\n", base_location, comparison_location);
		printf("\t[VERY LIKELY] duplicates; difference = %f\n", total_difference);
	}
	else if (total_difference <= 0.1) {
		printf("\n%s and %s\n", base_location, comparison_location);
		printf("\t[PROBABLY] duplicates; difference = %f\n", total_difference);
	}
	else if (total_difference <= 0.2) {
		printf("\n%s and %s\n", base_location, comparison_location);
		printf("\t[MAYBE] duplicates; difference = %f\n", total_difference);
	}	
	else {
		// printf("\n%s and %s\n", base_location, comparison_location);
		// printf("\t[NOT] duplicates; difference = %f\n", total_difference);
	}
}
