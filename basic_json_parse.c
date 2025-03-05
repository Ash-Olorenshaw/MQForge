#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "globals.h"

struct mapElement {
	char *key;
	char *val;
};

int split_respect_strings(char *string_item, char delimiter, char tokens_out[MAX_TOKEN_SIZE][MAX_JSON_KEYS]) {
	char current_string[MAX_TOKEN_SIZE] = {0};
	bool inside_string = false;
	int count = 0;

	for (int i = 0; i < strlen(string_item); i++) {
		char ch = string_item[i];
		
		if (ch == delimiter && !inside_string) {
			strcpy(tokens_out[count], current_string);
			count++;
			memset(current_string, 0, sizeof(current_string));
		}
		else if (i == (strlen(string_item) - 1)) {
			current_string[strlen(current_string)] = ch;
			current_string[strlen(current_string) + 1] = '\0';
			strcpy(tokens_out[count], current_string);
			count++;
			memset(current_string, 0, sizeof(current_string));
		}
		else if (ch == '"') {
			inside_string = !inside_string;
			current_string[strlen(current_string)] = ch;
			current_string[strlen(current_string) + 1] = '\0';
		}
		else {
			current_string[strlen(current_string)] = ch;
			current_string[strlen(current_string) + 1] = '\0';
		}
	}
	return 200;
}

void read_basic_json(char *target_file, int *json_size, struct mapElement (*json_items)[MAX_JSON_KEYS]) {
	char buffer[MAX_JSON_FILE_SIZE];
	FILE *file_ptr = fopen(target_file, "r");
	char line_items[MAX_TOKEN_SIZE][MAX_JSON_KEYS];// = (char**)(memory_store[0] = malloc(2 * sizeof(char*)));

	if (file_ptr) {
		fread(buffer, MAX_JSON_FILE_SIZE, 1, file_ptr);
		fclose(file_ptr);
	}
	else {
		fprintf(stderr, "Err - file can't be opened: '%s' \n", target_file);
		exit(1);
	}

	trim(buffer);
	if (buffer[0] != '{' || buffer[strlen(buffer) - 1] != '}') {
		fprintf(stderr, "Err - Unable to find leading and trailing {}s in json file %s.", target_file);
		exit(1);
	}

	*json_size = string_occurences(buffer, ',') + 1;
	char lines[MAX_TOKEN_SIZE][MAX_JSON_KEYS];

	int lines_success = split_respect_strings(buffer, ',', lines);

	if (lines_success == 200) {
		for (int i = 0; i < *json_size; i++) {
			char *trimmed_item = trim(trim_char(trim_char(lines[i], '{'), '}'));
			int line_items_success = split_respect_strings(trimmed_item, ':', line_items);

			if (line_items_success == 200) {
				char *key = trim(line_items[0]);
				char *item = trim(line_items[1]);

				if (string_occurences(line_items[0], '"') > 1) {
					int arg_elem = string_to_bool(item);
					extract_delimited_string(key, '"');

					if (string_occurences(item, '"') > 1) {
						(*json_items)[i].key = strdup(key);
						(*json_items)[i].val = strdup(item);
					}
					else if (arg_elem > -1) {
						(*json_items)[i].key = strdup(key);
						(*json_items)[i].val = strdup(item);
					}
					else {
						fprintf(stderr, "Err - item '%s' is neither a string or bool.\n", item);
						exit(1);
					}
				}
				else {
					fprintf(stderr, "Err - key '%s' cannot be interpreted as a string.\n", key);
					exit(1);
				}
			}
			else {
				fprintf(stderr, "Err - failed to split item '%s'\n", trimmed_item);
				exit(1);
			}
		}
	}
	else {
		fprintf(stderr, "Err - failed to allocate memory when reading '%s'\n", buffer);
		exit(1);
	}
}

