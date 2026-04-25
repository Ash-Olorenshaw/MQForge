#ifndef __FILE_UTILS_H
#define __FILE_UTILS_H

#include <stdbool.h>
#include "../globals.h"

int is_directory(const char *path);
bool file_exists(char *filename);
int convert_wine_path(char *path, char final_string[MAX_TOKEN_SIZE]);

void list_files(const char *basePath, array *files, bool recurse);
int search_dir_for_ext(char *target_path, char *extension, array *files, bool recurse);
void search_PATH_for_ext(char *extension, array *additonal_dirs, array **files);

#endif
