#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

#include "../utils/string.h"
#include "../array.h"
#include "../globals.h"

#ifdef _WIN32
#include "../sys_interactions_windows.h"
#else
#include "../sys_interactions_linux.h"
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

void list_files(const char *basePath, array *files, bool recurse) {
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

			if (recurse)
				list_files(path, files, true);

			if (!is_directory(path)) {
				if (files->count < files->size - 1) {
					if (files->array[files->count] == NULL)
						files->array[files->count++] = strdup(path);
					else
						strcpy(files->array[files->count++], path);
				}
			}
			free(path);
		}
	}
	closedir(dir);
}

int search_dir_for_ext(const char *target_path, const char *extension, array *files, bool recurse) {
	array *all_files = NEW_ARRAY(MAX_ARRAY_SIZE);
	list_files(target_path, all_files, recurse);

	int i = 0;
	char *elem;
	ARRAY_FOREACH(elem, all_files, i) {
		array *lines = NEW_ARRAY(MAX_ARRAY_SIZE);

		int splits = split_string(elem, '.', lines);
		if (splits > 0 && splits < MAX_ARRAY_SIZE) {
			if (strcmp(lines->array[splits - 1], extension) == 0)
				files->array[files->count++] = strdup(elem);
		}
		free_array(&lines);
	}

	free_array(&all_files);

	return files->count;
}

void search_PATH_for_ext(const char *extension, array *additional_dirs, array **files) {
	if (files == NULL)
		return;

	search_dir_for_ext(work_area, extension, *files, false);

	array *path_dirs = NEW_ARRAY(MAX_ARRAY_SIZE);
	get_PATH(path_dirs);

	array *all_dirs = NEW_ARRAY(MAX_ARRAY_SIZE * 2);
	for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
		if (i < path_dirs->count && strcmp(path_dirs->array[i], "") != 0)
			all_dirs->array[all_dirs->count++] = strdup(path_dirs->array[i]);

		if (i < additional_dirs->count && strcmp(additional_dirs->array[i], "") != 0)
			all_dirs->array[all_dirs->count++] = strdup(additional_dirs->array[i]);
	}

	for (int idir = 0; idir < all_dirs->count; idir++) {
		array *new_dir_files = NEW_ARRAY(MAX_ARRAY_SIZE);
		search_dir_for_ext(all_dirs->array[idir], extension, new_dir_files, false);

		if (array_append(files, &new_dir_files) != 0) {
			free_array(&new_dir_files);
			break;
		}
	}

	free_array(&all_dirs);
	free_array(&path_dirs);
}
