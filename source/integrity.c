#include "../includes/integrity.h"

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

bool isImage(char *item_name)
{
	const unsigned minimum_filename_length = 5;
	const unsigned filename_length = strlen(item_name);

	// supporting just png and jpg/jpeg
	if (filename_length >= minimum_filename_length) {
		if (strncmp(&item_name[filename_length - 4], ".png", 4) == 0) {
			return true;
		}
		else if (strncmp(&item_name[filename_length - 4], ".jpg", 4) == 0) {
			return true;
		}
		else if (filename_length > minimum_filename_length && strncmp(&item_name[filename_length - 5], ".jpeg", 5) == 0) {
			return true;
		}

	}

	return false;
}

char *formatDirectoryName(char *source_directory_name)
{
	unsigned old_size = strlen(source_directory_name);
	char *formatted_directory_name = (char *) malloc(old_size + 1);
	strncpy(formatted_directory_name, source_directory_name, old_size);
	
	formatted_directory_name[old_size] = '/';
	formatted_directory_name[old_size + 1] = '\0';
	
	if (formatted_directory_name[old_size - 1] == '/') {
		formatted_directory_name[old_size] = '\0';
	}

	return formatted_directory_name;
}
