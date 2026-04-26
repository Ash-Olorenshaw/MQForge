#include <stdio.h>
#include <string.h>

#include "../utils/string.h"
#include "../globals.h"
#include "./dependencies.h"

map_element_array dependency_dict[MAX_ARRAY_SIZE] = {0};

map_element_array *get_dep_dict() {
	return dependency_dict;
}

int get_dep_dict_size() {
	int size = 0;
	for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
		if (dependency_dict[i].key != NULL && !string_isspace(dependency_dict[i].key))
			size++;
	}
	return size;
}

void print_deps(map_element_array dependency_dict[MAX_ARRAY_SIZE]) {
	printf("{\n");
	for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
		if (dependency_dict[i].key != NULL && strcmp(dependency_dict[i].key, "") != 0) {
			printf("\t\"%s\" : [", dependency_dict[i].key);
			for (int j = 0; j < MAX_ARRAY_SIZE; j++) {
				if (strcmp(dependency_dict[i].val[j], "") != 0) {
					printf("\"%s\", ", dependency_dict[i].val[j]);
				}
			}
			printf("], \n");
		}
	}
	printf("}\n");
}

bool check_deps_satisfied(array *target_deps, array *dependency_keys, array *unresolved_dependencies) {
	int found = 1;
	bool any_key_present;
	for (int i = 0; i < target_deps->count; i++) {
		if (strcmp(target_deps->array[i], "") != 0) {
			found++;
			any_key_present = false;
			for (int j = 0; j < dependency_keys->count; j++) {
				if (strcmp(dependency_keys->array[j], "") != 0) {
					if (strcmp(dependency_keys->array[j], target_deps->array[i]) == 0) {
						any_key_present = true;
						found--;
					}
				}
			}

			if (!any_key_present)
				unresolved_dependencies->array[unresolved_dependencies->count++] = strdup(target_deps->array[i]);
		}
	}

	return found == 1;
}

int get_dep_dict_keys(array *dependency_keys) {
	dependency_keys->count = 0;
	for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
		if (dependency_dict[i].key != NULL && strcmp(dependency_dict[i].key, "") != 0) {
			dependency_keys->array[dependency_keys->count++] = strdup(dependency_dict[i].key);
		}
	}
	return dependency_keys->count;
}


void change_deps_extensions(array *dependencies) {
	char *dep;
	int i;
	ARRAY_FOREACH(dep, dependencies, i) {
		if (strcmp(dep, "") != 0)
			dep = replace_str_ending(dep, ".ex4");
	}
}


int get_dep_dict_item_by_key(char *key, array *item) {
	for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
		if (strcmp(dependency_dict[i].key, "") != 0) {
			if (strcmp(dependency_dict[i].key, key) == 0) {
				for (int j = 0; j < MAX_ARRAY_SIZE; j++) {
					if (!string_isspace(dependency_dict[i].val[j]))
						item->array[item->count++] = strdup(dependency_dict[i].val[j]);
				}
				return 200;
			}
		}
	}
	return 400;
}

int delete_dep_dict_item_by_key(char *key) {
	for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
		if (strcmp(dependency_dict[i].key, "") != 0) {
			if (strcmp(dependency_dict[i].key, key) == 0) {
				dependency_dict[i].key = "";
				for (int j = 0; j < MAX_ARRAY_SIZE; j++) {
					if (strcmp(dependency_dict[i].val[j], "") != 0)
						strcpy(dependency_dict[i].val[j], "");
				}

				return 200;
			}
		}
	}
	return 400;
}
