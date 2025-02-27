#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "utils.h"
#include "file_utils.h"

int check_file_deps(char file[MAX_TOKEN_SIZE], char import_items[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE]) {
	char builtin_headers[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE] = {0};
	//printf("\topening file '%s'\n", file);
	int builtin_headers_count = search_dir_for_ext(default_header_location, "mqh", builtin_headers);
	//printf("\tFINISHED searching for mqh...\n");

	char buffer[MAX_TOKEN_SIZE];
	FILE* filePointer;
	int line_num = 0;

	filePointer = fopen(file, "r");

	while(fgets(buffer, MAX_TOKEN_SIZE, filePointer)) {
		// line by line...
		trim(buffer);
		char *buffer_copy = strdup(buffer);

		if (strlen(buffer) < 7) {
			continue;
		}

		char substring[MAX_TOKEN_SIZE] = {0};
		if (strcmp(get_substring(buffer, 0, 7, substring), "#import") == 0) {
			if (string_occurences(get_substring(buffer, 7, strlen(buffer) - 1, substring), '<') < 1 && string_occurences(get_substring(buffer, 7, strlen(buffer) - 1, substring), '>') < 1) {
				//printf("\tno '<'s... %s\n", substring);
				char *import_item = strdup(extract_delimited_string(trim(get_substring(buffer_copy, 7, strlen(buffer) - 1, substring)), '"'));
				//printf("\textracted import_item '%s'...\n", import_item);
				if (import_item && !string_isspace(import_item)) {
					char *extension = get_substring(import_item, strlen(import_item) - 4, strlen(import_item) - 1, substring);
					//printf("\textracted extension %s from %s...\n", extension, import_item);
					if (strcmp(extension, ".ex4") == 0) {
						char *ending_solved_import_item = replace_str_ending(import_item, ".mq4");

						strcpy(import_items[line_num], ending_solved_import_item);
					}
					else if (strcmp(extension, ".mqh") == 0) {
						//printf("\tfound header...\n");
						if (!value_in_string_array(import_item, available_headers, *(&available_headers + 1) - available_headers)) {
							printf("Err - unable to find .mqh header file '%s' referenced in %s (line: %d)\n", import_item, file, line_num);
							return 400;
						}
					}
					else if (strcmp(string_lower(extension), ".dll") == 0) {
						if (!value_in_string_array(import_item, available_dlls, *(&available_dlls + 1) - available_dlls)) {
							printf("Err - unable to find dll '%s' referenced in %s (line: %d)\n", import_item, file, line_num);
							return 400;
						}
					}
					else {
						printf("Err - unrecognised imported file extension: '%s' for '%s' in '%s'\n", extension, buffer, file);
						return 400;
					}
				}
			}
			else {
				char *import_item = rtrim_char(ltrim_char(trim(get_substring(buffer, 7, strlen(buffer) - 1, substring)), '<'), '>');
				if (import_item && !string_isspace(import_item)) {
					if (!value_in_string_array(import_item, builtin_headers, *(&builtin_headers + 1) - builtin_headers)) {
						//printf("Err - unable to find .mqh header file '%s referenced in %s(line: %d)'\n", import_item, file, line_num);
						return 400;
					}
				}
			}
		}
		line_num++;
	}

	fclose(filePointer);

	return 200;
}

