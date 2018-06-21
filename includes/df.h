#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>
#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// each channel in each pixel will have a value from 1 to CHANNEL_MAX_VALUE
static const double CHANNEL_MAX_VALUE = 256.0;
static const int MAX_IMAGES_PER_DIR = 60;

bool verifyParams(int argc, char *source_directory);
void printImageDetails(Image *image);
void initializeImages(Image *images[], char *source_directory, int *number_of_images);
void storeChannels(Image *image);
void printChannels(Image *image);
void compareChannels(Image *base, Image *comparison);
void analyzeChannelDifference(double total_difference, char *base_location, char *comparison_location);
