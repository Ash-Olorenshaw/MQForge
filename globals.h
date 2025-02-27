#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdbool.h>

#define MAX_TOKEN_SIZE 256
#define MAX_ARRAY_SIZE 500
#define MAX_PATH_LEN 1024
#define MAX_JSON_KEYS 256
#define MAX_JSON_FILE_SIZE 4096

struct map_element_array {
	char *key;
	char val[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE];
};

extern char available_headers[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE];
extern char available_dlls[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE];
extern char *market_platform;
extern char *meta_editor;
extern char *default_header_location;
extern bool use_wine;
extern bool colourful;
extern bool suppress_launch_errors;
extern bool use_PATH;
extern char *alt_settings_file;
extern struct map_element_array dependency_dict[MAX_ARRAY_SIZE];

#endif
