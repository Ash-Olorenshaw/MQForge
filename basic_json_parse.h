#ifndef BASICJSON_H
#define BASICJSON_H

#include <stdbool.h>
#include "globals.h"

struct mapElement {
	char *key;
	char *val;
};

int split_respect_strings(char *string_item, char delimiter, char tokens_out[MAX_TOKEN_SIZE][MAX_JSON_KEYS]);
void read_basic_json(char *target_file, int *json_size, struct mapElement (*json_items)[MAX_JSON_KEYS]);

#endif
