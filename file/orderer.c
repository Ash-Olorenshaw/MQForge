#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "importer.h"
#include "dependencies.h"
#include "../utils/string.h"
#include "../globals.h"


int create_file_order(array *files, array **dependency_tree, int *file_num, array * available_dlls, array *available_headers) {
	int dependency_files = 0;
	int final_success = 200;

	array *precompiled_files = NEW_ARRAY(MAX_ARRAY_SIZE);
	array *additional_search_dirs = NEW_ARRAY(MAX_ARRAY_SIZE);
	search_PATH_for_ext("ex4", additional_search_dirs, &precompiled_files);
	map_element_array *dependency_dict = get_dep_dict();
	free_array(&additional_search_dirs);

	char *file;
	int ifile;
	ARRAY_FOREACH(file, files, ifile) {
		if (strcmp(file, "") != 0) {
			dependency_files++;
			array *deps = NEW_ARRAY(MAX_ARRAY_SIZE);
			int success = check_file_deps(file, deps, available_dlls, available_headers);
			if (success == 200) {
				char *dep;
				int idep = 0;
				ARRAY_FOREACH(dep, deps, idep) {
					dep = trim(dep);
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
				}
				free_array(&deps);
				dependency_dict[ifile].key = file;
			}
			else {
				free_array(&deps);
				goto exit_fail;
			}
		}
	}

	if (dependency_files < 1) {
		goto exit_fail;
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
		array *dependency_keys = NEW_ARRAY(MAX_ARRAY_SIZE);
		int num_keys = get_dep_dict_keys(dependency_keys);

		for (int i = 0; i < num_keys; i++) {
			array * target_dependencies = NEW_ARRAY(MAX_ARRAY_SIZE);
			int success = get_dep_dict_item_by_key(dependency_keys->array[i], target_dependencies);
			if (success == 200) {
				array *unresolved_dependencies = NEW_ARRAY(MAX_ARRAY_SIZE);
				bool satisfied = check_deps_satisfied(target_dependencies, (*dependency_tree), unresolved_dependencies);
				if (satisfied == true) {
					(*dependency_tree)->array[(*dependency_tree)->count++] = strdup(dependency_keys->array[i]);
					delete_dep_dict_item_by_key(dependency_keys->array[i]);
					(*file_num)++;
					free_array(&unresolved_dependencies);
					free_array(&target_dependencies);
					break;
				}
				else {
					array *unresolved_compiled_dependencies = NEW_ARRAY(MAX_ARRAY_SIZE);
					change_deps_extensions(unresolved_dependencies);
					bool satisfied = check_deps_satisfied(unresolved_dependencies, precompiled_files, unresolved_compiled_dependencies);
					if (satisfied) {
						(*dependency_tree)->array[(*dependency_tree)->count++] = strdup(dependency_keys->array[i]);
						delete_dep_dict_item_by_key(dependency_keys->array[i]);
						(*file_num)++;
						free_array(&unresolved_compiled_dependencies);
						free_array(&unresolved_dependencies);
						free_array(&target_dependencies);
						break;
					}
					free_array(&unresolved_compiled_dependencies);
				}
				free_array(&unresolved_dependencies);
			}
			free_array(&target_dependencies);
		}

		free_array(&dependency_keys);
		prev_len = current_len;
	}

	free_array(&precompiled_files);
	return final_success;

	exit_fail:
		free_array(&precompiled_files);
		return 400;
}
