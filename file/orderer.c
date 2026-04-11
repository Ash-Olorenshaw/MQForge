#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "importer.h"
#include "dependencies.h"
#include "../utils/string.h"
#include "../globals.h"


int create_file_order(char files[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE], char dependency_tree[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE], int *file_num) {
	int dependency_tree_ptr = 0;
	int dependency_files = 0;
	int final_success = 200;

	char precompiled_files[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE] = {0};
	char additional_search_dirs[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE] = {0};
	int precompiled_files_count = search_PATH_for_ext("ex4", additional_search_dirs, precompiled_files);

	for (int ifile = 0; ifile < MAX_ARRAY_SIZE; ifile++) {
		if (files[ifile] != NULL && strcmp(files[ifile], "") != 0) {
			dependency_files++;
			char deps[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE] = {0};
			int success = check_file_deps(files[ifile], deps);
			if (success == 200) {
				for (int idep = 0; idep < MAX_ARRAY_SIZE; idep++) {
					char *dep = trim(strdup(deps[idep]));
					if (dep != NULL && strcmp(dep, "") != 0) {
						if (dep[1] != '\\' && dep[1] != '/' && dep[1] != ':') {
							char new_dep[MAX_TOKEN_SIZE] = "./";
							strcat(new_dep, dep);
							strcpy(dependency_dict[ifile].val[idep], new_dep);
						}
						else {
							strcpy(dependency_dict[ifile].val[idep], dep);
						}
					}
					free(dep);
				}
				dependency_dict[ifile].key = files[ifile];
			}
			else {
				return 400;
			}
		}
	}

	if (dependency_files < 1) {
		return 400;
	}

	int prev_len = get_dep_dict_size() + 1;
	int current_len;
	printf("\nDependency/compile tree:\n");
	print_deps(dependency_dict);

	while ((current_len = get_dep_dict_size()) > 0) {
		if (current_len >= prev_len) {
			printf("Broken dependencies:\n");
			print_deps(dependency_dict);
			final_success = 400;
			break;
		}
		char dependency_keys[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE] = {0};
		int num_keys = get_dep_dict_keys(dependency_keys);

		for (int i = 0; i < num_keys; i++) {
			char target_dependencies[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE] = {0};
			int success = get_dep_dict_item_by_key(dependency_keys[i], target_dependencies);
			if (success == 200) {
				char unresolved_dependencies[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE] = {0};
				bool satisfied = check_deps_satisfied(target_dependencies, dependency_tree, unresolved_dependencies);
				if (satisfied == true) {
					strcpy(dependency_tree[dependency_tree_ptr++], dependency_keys[i]);
					delete_dep_dict_item_by_key(dependency_keys[i]);
					(*file_num)++;
					break;
				}
				else {
					char unresolved_compiled_dependencies[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE] = {0};
					change_deps_extensions(unresolved_dependencies);
					bool satisfied = check_deps_satisfied(unresolved_dependencies, precompiled_files, unresolved_compiled_dependencies);
					if (satisfied) {
						strcpy(dependency_tree[dependency_tree_ptr++], dependency_keys[i]);
						delete_dep_dict_item_by_key(dependency_keys[i]);
						(*file_num)++;
						break;
					}
				}
			}
		}

		prev_len = current_len;
	}

	return final_success;
}
