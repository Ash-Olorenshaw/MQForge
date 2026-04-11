#include <stdio.h>
#include <string.h>

#include "../utils/string.h"
#include "../globals.h"

int get_dep_dict_size() {
	int size = 0;
	for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
		if (dependency_dict[i].key != NULL && strcmp(dependency_dict[i].key, "") != 0) {
			size++;
		}
	}
	return size;
}

void print_deps(struct map_element_array dependency_dict[MAX_ARRAY_SIZE]) {
	printf("{\n");
	for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
		if (dependency_dict[i].key != NULL) {
			printf("\t\"%s\" : [", dependency_dict[i].key);
			for (int j = 0; j < MAX_ARRAY_SIZE; j++) {
				if (strcmp(dependency_dict[i].val[j], "") != 0 && dependency_dict[i].val[j] != NULL) {
					printf("\"%s\", ", dependency_dict[i].val[j]);
				}
			}
			printf("], \n");
		}
	}
	printf("}\n");
}

bool check_deps_satisfied(char target_deps[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE], char dependency_keys[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE], char unresolved_dependencies[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE]) {
	int found = 1;
	int final_ptr = 0;
	bool any_key_present;
	for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
		if (target_deps[i] != NULL && strcmp(target_deps[i], "") != 0) {
			found++;
			any_key_present = false;
			for (int j = 0; j < MAX_ARRAY_SIZE; j++) {
				if (dependency_keys[j] != NULL && strcmp(dependency_keys[j], "") != 0) {
					if (strcmp(dependency_keys[j], target_deps[i]) == 0) {
						any_key_present = true;
						found--;
					}
				}
			}

			if (!any_key_present)
				strcpy(unresolved_dependencies[final_ptr++], target_deps[i]);
		}
	}

	return found == 1;
}

int get_dep_dict_keys(char dependency_keys[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE]) {
	int index = 0;
	for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
		if (dependency_dict[i].key != NULL && strcmp(dependency_dict[i].key, "") != 0) {
			strcpy(dependency_keys[index++], dependency_dict[i].key);
		}
	}
	return index;
}


void change_deps_extensions(char dependencies[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE]) {
	for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
		if (dependencies[i] != NULL && strcmp(dependencies[i], "") != 0) {
			char *new_str = replace_str_ending(dependencies[i], ".ex4");
			strcpy(dependencies[i], new_str);
		}
	}
}


int get_dep_dict_item_by_key(char *key, char item[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE]) {
	for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
		if (dependency_dict[i].key != NULL && strcmp(dependency_dict[i].key, "") != 0) {
			if (strcmp(dependency_dict[i].key, key) == 0) {
				for (int j = 0; j < MAX_ARRAY_SIZE; j++) {
					strcpy(item[j], dependency_dict[i].val[j]);
				}

				return 200;
			}
		}
	}
	return 400;
}

int delete_dep_dict_item_by_key(char *key) {
	for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
		if (dependency_dict[i].key != NULL && strcmp(dependency_dict[i].key, "") != 0) {
			if (strcmp(dependency_dict[i].key, key) == 0) {
				dependency_dict[i].key = NULL;
				for (int j = 0; j < MAX_ARRAY_SIZE; j++) {
					if (dependency_dict[i].val[j] != NULL && strcmp(dependency_dict[i].val[j], "") != 0) {
						strcpy(dependency_dict[i].val[j], "");
					}
				}

				return 200;
			}
		}
	}
	return 400;
}
