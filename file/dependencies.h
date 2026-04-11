#ifndef __FILE_DEPENDENCIES_H
#define __FILE_DEPENDENCIES_H

#include "../globals.h"

int get_dep_dict_size();
void print_deps(struct map_element_array dependency_dict[MAX_ARRAY_SIZE]);
bool check_deps_satisfied(char target_deps[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE], char dependency_keys[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE], char unresolved_dependencies[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE]);
int get_dep_dict_keys(char dependency_keys[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE]);
void change_deps_extensions(char dependencies[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE]);
int get_dep_dict_item_by_key(char *key, char item[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE]);
int delete_dep_dict_item_by_key(char *key);

#endif
