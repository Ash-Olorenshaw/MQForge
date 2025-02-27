#include <stdbool.h>
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


int isDirectory(const char *path) {
	struct stat statbuf;
	if (stat(path, &statbuf) != 0)
		return 0;
	return S_ISDIR(statbuf.st_mode);
}

void listFilesRecursively(const char *basePath, char files[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE], int *files_size)
{
	char path[1000];
	struct dirent *dp;
	DIR *dir = opendir(basePath);

	// Unable to open directory stream
	if (!dir){
		return;
	}

	while ((dp = readdir(dir)) != NULL)
	{
		if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
		{
			strcpy(path, basePath);
			strcat(path, "/");
			strcat(path, dp->d_name);

			if (!isDirectory(path)) {
				if (*files_size < MAX_ARRAY_SIZE - 1) {
					strcpy(files[(*files_size)++], dp->d_name);
				}
			}

			listFilesRecursively(path, files, files_size);
		}
	}
	closedir(dir);
}

int search_dir_for_ext(const char *target_path, const char *extension, char files[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE]) {
	//printf("\t\tstarting search in %s for %s\n", target_path, extension);
	char all_files[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE] = {0};
	int fileptr = 0;
	//printf("\t\trecursive list:\n");
	listFilesRecursively(target_path, all_files, &fileptr);
	//printf("\t\tfinished recursive search\n");

	int files_found = 0;

	for (int i = 0; i < fileptr; i++) {
		char lines[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE] = {0};
		//printf("\t\tloop... %d\n", i);

		int splits = split_string(all_files[i], '.', lines);
		//printf("\t\tsplit with num %d... \n", splits);
		if (splits > 0 && files_found < MAX_ARRAY_SIZE) {
			if (strcmp(lines[splits - 1], extension) == 0) {
				//printf("\t\tcopying %s... \n", all_files[i]);
				strcpy(files[files_found++], all_files[i]);
				//printf("\t\tcopy finished... %s\n", files[files_found]);
			}
		}
	}

	return files_found;
}

int search_PATH_for_ext(const char *extension, char additional_dirs[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE], char files[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE]) {
	int current_files_found = search_dir_for_ext(".", extension, files);
	char path_dirs[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE] = {0};
	get_PATH(path_dirs);

	int additional_dirs_size = *(&additional_dirs + 1) - additional_dirs;

	char all_dirs[MAX_ARRAY_SIZE * 2][MAX_TOKEN_SIZE] = {0};
	int all_dirs_index = 0;
	for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
		if (path_dirs[i] != NULL && strcmp(path_dirs[i], "") != 0) {
			strcpy(all_dirs[all_dirs_index++], path_dirs[i]);
		}

		if (additional_dirs_size > 0 && additional_dirs[i] != NULL && strcmp(additional_dirs[i], "") != 0) {
			strcpy(all_dirs[all_dirs_index++], additional_dirs[i]);
		}
	}

	int insertion_ptr = current_files_found;
	for (int idir = 0; idir < all_dirs_index + 1; idir++) {
		char new_dir_files[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE] = {0};
		//memset(new_dir_files, 0, sizeof(new_dir_files));
		int new_files_found = search_dir_for_ext(all_dirs[idir], extension, files);
		if (new_files_found + insertion_ptr < MAX_ARRAY_SIZE) {
			memcpy(files + insertion_ptr, new_dir_files, new_files_found * sizeof(char *));
			insertion_ptr += new_files_found;
		}
	}
	//printf("finished search...\n");

	return insertion_ptr;
}
