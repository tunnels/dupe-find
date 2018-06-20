typedef struct image_struct_t {
	unsigned char *image_data;
	char location[FILENAME_MAX * 2];
	int width;
	int height;
	int number_of_pixels;
	int components_per_pixel;
	long *components;
	struct image_struct_t *next;
} Image;
