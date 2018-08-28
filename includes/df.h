#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// each channel in each pixel will have a value from 1 to CHANNEL_MAX_VALUE
#define CHANNEL_MAX_VALUE 256.0
#define MAX_IMAGES_PER_DIR 2000

typedef struct image_struct_t {
	unsigned char *image_data;
	char location[FILENAME_MAX * 2];
	int width;
	int height;
	int number_of_pixels;
	int channels_per_pixel;
	long long *channels;
} Image;


bool verifyParams(int argc, char *source_directory);
void printImageDetails(Image *image);
void initializeImages(Image *images[], char *source_directory, int *number_of_images);
void storeChannels(Image *image);
void printChannels(Image *image);
void compareChannels(Image *base, Image *comparison);
void analyzeChannelDifference(double total_difference, char *base_location, char *comparison_location);
