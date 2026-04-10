#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "globals.h"

#ifdef _WIN32
#include "sys_interactions_windows.h"
#else
#include "sys_interactions_linux.h"
#endif


int is_directory(const char *path) {
	struct stat statbuf;
	if (stat(path, &statbuf) != 0)
		return 0;
	return S_ISDIR(statbuf.st_mode);
}

bool file_exists(char *filename) {
	struct stat buffer;
	return stat(filename, &buffer) == 0;
}

int convert_wine_path(char *path, char final_string[MAX_TOKEN_SIZE]) {
	if (strstr(path, "drive_c") != NULL) {
		char *temp_path = strdup(path);
		strcpy(final_string, "C:");
		temp_path = split_get_second_half(temp_path, "drive_c");
		strncat(final_string, temp_path, MAX_TOKEN_SIZE);
		return 200;
	}
	return 400;
}

void list_files(const char *basePath, char files[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE], int *files_size, bool recurse) {
	char *path;
	struct dirent *dp;
	DIR *dir = opendir(basePath);

	if (!dir)
		return;

	while ((dp = readdir(dir)) != NULL) {
		if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
			path = malloc(sizeof(char) * strlen(basePath) + 1 + strlen(dp->d_name) + 1);
			strcpy(path, basePath);
			strcat(path, "/");
			strcat(path, dp->d_name);

			if (!is_directory(path)) {
				if (*files_size < MAX_ARRAY_SIZE - 1) {
					strcpy(files[(*files_size)++], path);
				}
			}

			if (recurse)
				list_files(path, files, files_size, true);
			free(path);
		}
	}
	closedir(dir);
}

int search_dir_for_ext(const char *target_path, const char *extension, char files[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE], bool recurse) {
	char all_files[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE] = {0};
	int fileptr = 0;

	list_files(target_path, all_files, &fileptr, recurse);

	int files_found = 0;

	for (int i = 0; i < fileptr; i++) {
		char lines[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE] = {0};

		int splits = split_string(all_files[i], '.', lines);
		if (splits > 0 && files_found < MAX_ARRAY_SIZE) {
			if (strcmp(lines[splits - 1], extension) == 0) {
				strcpy(files[files_found++], all_files[i]);
				// printf("Searching file %s\n", files[files_found]);
			}
		}
	}

	return files_found;
}

int search_PATH_for_ext(const char *extension, char additional_dirs[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE], char files[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE]) {
	int current_files_found = search_dir_for_ext(work_area, extension, files, false);
	char path_dirs[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE] = {0};
	get_PATH(path_dirs);

	char all_dirs[MAX_ARRAY_SIZE * 2][MAX_TOKEN_SIZE] = {0};
	int all_dirs_index = 0;
	for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
		if (path_dirs[i] != NULL && strcmp(path_dirs[i], "") != 0) {
			strcpy(all_dirs[all_dirs_index++], path_dirs[i]);
		}

		if (additional_dirs[i] != NULL && strcmp(additional_dirs[i], "") != 0) {
			strcpy(all_dirs[all_dirs_index++], additional_dirs[i]);
		}
	}

	int insertion_ptr = current_files_found;
	for (int idir = 0; idir < all_dirs_index + 1; idir++) {
		char new_dir_files[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE] = {0};
		// printf("Searching directory %s for extension %s\n", all_dirs[idir], extension);
		int new_files_found = search_dir_for_ext(all_dirs[idir], extension, files, false);
		if (new_files_found + insertion_ptr < MAX_ARRAY_SIZE) {
			memcpy(files + insertion_ptr, new_dir_files, new_files_found * sizeof(char *));
			insertion_ptr += new_files_found;
		}
	}

	return insertion_ptr;
}
