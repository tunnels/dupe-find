#include "../includes/dupe_find.h"
#include "../includes/integrity.h"

int main(int argc, char **argv)
{
	if (!verifyParams(argc, argv)) {
		return -1;
	}

	unsigned number_of_images;
	Image *images[MAX_IMAGES_TO_PROCESS];

	for (unsigned i = 1; i < argc; i++) {
		initializeImages(&images[0], argv[i], &number_of_images);
	}

	for (unsigned i = 0; i < number_of_images; i++) {
		for (unsigned j = i + 1; j < number_of_images; j++) {
			compareChannels(images[i], images[j]);
		}

		free(images[i]);
	}

	return 0;
}

void printImageDetails(Image *image)
{
	printf("\nlocation: %s\nw:%d, h:%d, c:%d\n", image->location, image->width, image->height, image->channels_per_pixel);
}

void initializeImages(Image *images[MAX_IMAGES_TO_PROCESS], char *source_directory, unsigned *number_of_images)
{
	struct dirent *directory_entry;

	DIR *directory_pointer = opendir(source_directory);
	if (directory_pointer != NULL) {
		while ((directory_entry = readdir(directory_pointer))) {
			// skip hidden files
			if (!strncmp(directory_entry->d_name, ".", 1)) {
				continue;
			}

			bool isAnImage = isImage(directory_entry->d_name);
			char *formatted_directory_name = formatDirectoryName(source_directory);

			if (!isAnImage) {
				if (directory_entry->d_type == DT_DIR) {
					char *source_directory_subdir = malloc(strlen(formatted_directory_name) + strlen(directory_entry->d_name) + 1);
					strcat(source_directory_subdir, formatted_directory_name);
					strcat(source_directory_subdir, directory_entry->d_name);
					initializeImages(images, source_directory_subdir, number_of_images);
				}
				else if (directory_entry->d_type == DT_LNK) {
					// handle symlinks
				}
				continue;
			}

			Image *new_image = createImage(formatted_directory_name, directory_entry->d_name);
			storeChannels(new_image);
			free(new_image->image_data);

			if (*number_of_images < MAX_IMAGES_TO_PROCESS) {
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

Image *createImage(char *directory_name, char *item_name)
{
	Image *new_image = (Image *) malloc(sizeof(*new_image));
	char image_path[FILENAME_MAX * 2];

	// // image_path is directory_name + current_filename
	memset(image_path, 0, FILENAME_MAX * 2);
	strncpy(image_path, directory_name, strlen(directory_name));
	strcat(image_path, item_name);
	strncpy(new_image->location, image_path, FILENAME_MAX * 2);

	new_image->image_data = stbi_load(new_image->location, &new_image->width, &new_image->height, &new_image->channels_per_pixel, 0);
	new_image->channels = calloc(sizeof(long long), new_image->channels_per_pixel);

	return new_image;
}

void storeChannels(Image *image)
{
	unsigned number_of_pixels = image->width * image->height;
	for (unsigned i = 0; i < (number_of_pixels * image->channels_per_pixel); i++) {
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
	for (unsigned i = 0; i < image->channels_per_pixel; i++) {
		printf("channel %d: %lld\n", i, image->channels[i]);
	}

	printf("\n");
}

void compareChannels(Image *base, Image *comparison)
{
	unsigned channels_to_consider = (base->channels_per_pixel <= comparison->channels_per_pixel) ? base->channels_per_pixel : comparison->channels_per_pixel;

	double base_denominator = base->width * base->height * channels_to_consider * CHANNEL_MAX_VALUE;
	double comparison_denominator = comparison->width * comparison->height * channels_to_consider * CHANNEL_MAX_VALUE;

	double base_calculated_value = 0;
	double comparison_calculated_value = 0;
	double total_difference = 0;

	for (unsigned i = 0; i < channels_to_consider; i++) {
		base_calculated_value = (base->channels[i] / base_denominator) * 100;
		comparison_calculated_value = (comparison->channels[i] / comparison_denominator) * 100;
		total_difference += fabs(comparison_calculated_value - base_calculated_value);
	}

	analyzeChannelDifference(total_difference, base->location, comparison->location);
}

void analyzeChannelDifference(double total_difference, char *base_location, char *comparison_location)
{
	if (total_difference == 0) {
		printf("[Perfect Duplicates]\n");
	}
	else if (total_difference <= 0.01) {
		printf("[Very Likely | %f]\n", total_difference);
	}
	else if (total_difference <= 0.1) {
		printf("[Probably | %f]\n", total_difference);
	}
	else if (total_difference <= 0.2) {
		printf("[Maybe | %f]\n", total_difference);
	}	
	else {
		// printf("[Nope | %f]\n", total_difference);
		return;
	}

	printf("%s\n", base_location);
	printf("%s\n\n", comparison_location);
}
