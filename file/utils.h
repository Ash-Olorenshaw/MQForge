#ifndef __FILE_UTILS_H
#define __FILE_UTILS_H

#include <stdbool.h>
#include "../globals.h"

int is_directory(const char *path);
bool file_exists(char *filename);
int convert_wine_path(char *path, char final_string[MAX_TOKEN_SIZE]);

void list_files(char *basePath, char files[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE], int *filelen, bool recurse);
int search_dir_for_ext(char *target_path, char *extension, char files[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE], bool recurse);
int search_PATH_for_ext(char *extension, char additonal_dirs[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE], char files[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE]);

#endif
