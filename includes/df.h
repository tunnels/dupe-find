#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>
#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// each component in each pixel will have a value from 1 to COMPONENT_MAX_VALUE
static const double COMPONENT_MAX_VALUE = 256.0;
static const int MAX_IMAGES_PER_DIR = 5;

bool verifyParams(int argc, char *source_directory);
void printImageDetails(Image *image);
void initializeImages(Image *images[], char *source_directory, int *number_of_images);
void storeComponents(Image *image);
void printComponents(Image *image);
void compareComponents(Image *base, Image *comparison);
