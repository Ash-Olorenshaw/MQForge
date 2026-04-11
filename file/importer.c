#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../globals.h"
#include "../utils/string.h"

int check_file_deps(char file[MAX_TOKEN_SIZE], char import_items[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE]) {
	char buffer[MAX_TOKEN_SIZE];
	FILE* filePointer;
	int line_num = 0;
	int result = 200;
	char *buffer_copy = NULL;

	filePointer = fopen(file, "r");

	while(fgets(buffer, MAX_TOKEN_SIZE, filePointer)) {
		// line by line...
		if (strlen(buffer) < 7) {
			continue;
		}

		trim(buffer);
		buffer_copy = strdup(buffer);

		char substring[MAX_TOKEN_SIZE] = {0};
		if (strcmp(get_substring(buffer, 0, 7, substring), "#import") == 0) {
			if (string_occurences(get_substring(buffer, 7, strlen(buffer) - 1, substring), '<') < 1 && string_occurences(get_substring(buffer, 7, strlen(buffer) - 1, substring), '>') < 1) {
				char *import_item = strdup(extract_delimited_string(trim(get_substring(buffer_copy, 7, strlen(buffer) - 1, substring)), '"'));
				if (import_item && !string_isspace(import_item)) {
					char *extension = get_substring(import_item, strlen(import_item) - 4, strlen(import_item) - 1, substring);
					if (strcmp(extension, ".ex4") == 0) {
						char *ending_solved_import_item = replace_str_ending(import_item, ".mq4");

						strcpy(import_items[line_num], ending_solved_import_item);
					}
					else if (strcmp(extension, ".mqh") == 0) {
						if (!value_in_string_array(import_item, available_headers, *(&available_headers + 1) - available_headers)) {
							fprintf(stderr, "Err - unable to find .mqh header file '%s' referenced in %s (line: %d)\n", import_item, file, line_num);
							result = 400;
							break;
						}
					}
					else if (strcmp(string_lower(extension), ".dll") == 0) {
						if (!value_in_string_array(import_item, available_dlls, *(&available_dlls + 1) - available_dlls)) {
							fprintf(stderr, "Err - unable to find dll '%s' referenced in %s (line: %d)\n", import_item, file, line_num);
							result = 400;
							break;
						}
					}
					else {
						fprintf(stderr, "Err - unrecognised imported file extension: '%s' for '%s' in '%s'\n", extension, buffer, file);
						result = 400;
						break;
					}
				}
				free(import_item);
			}
			else {
				char *import_item = rtrim_char(ltrim_char(trim(get_substring(buffer, 7, strlen(buffer) - 1, substring)), '<'), '>');
				if (import_item && !string_isspace(import_item)) {
					if (!value_in_string_array(import_item, available_headers, *(&available_headers + 1) - available_headers)) {
						fprintf(stderr, "Err - unable to find .mqh header file '%s referenced in %s(line: %d)'\n", import_item, file, line_num);
						result = 400;
						break;
					}
				}
			}
		}
		line_num++;
		if (buffer_copy != NULL) {
			free(buffer_copy);
			buffer_copy = NULL;
		}
	}

	if (buffer_copy != NULL) {
		free(buffer_copy);
		buffer_copy = NULL;
	}

	fclose(filePointer);

	return result;
}

