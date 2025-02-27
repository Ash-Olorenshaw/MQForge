#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdbool.h>
#include "globals.h"

void listFilesRecursively(char *basePath, char files[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE], int *filelen);
int search_dir_for_ext(char *target_path, char *extension, char files[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE]);
int search_PATH_for_ext(char *extension, char additonal_dirs[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE], char files[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE]);

#endif
