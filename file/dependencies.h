#ifndef __FILE_DEPENDENCIES_H
#define __FILE_DEPENDENCIES_H

#include "../globals.h"

typedef struct {
	char *key;
	char val[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE];
} map_element_array;

map_element_array *get_dep_dict(void);
int get_dep_dict_size(void);
void print_deps(map_element_array dependency_dict[MAX_ARRAY_SIZE]);
bool check_deps_satisfied(array *target_deps, array *dependency_keys, array *unresolved_dependencies);
int get_dep_dict_keys(array *dependency_keys);
void change_deps_extensions(array *dependencies);
int get_dep_dict_item_by_key(char *key, array *item);
int delete_dep_dict_item_by_key(char *key);

#endif
