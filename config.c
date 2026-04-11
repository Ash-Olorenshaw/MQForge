#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "file/utils.h"
#include "utils/string.h"
#include "globals.h"
#include "cJSON.h"

void process_core_flags(int argc, char *argv[]) {
	for (int i = 0; i < argc; i++) {
		if (check_arg_equals(argv[i], "-h", "--help", NULL)) {
			printf(VERSION_STRING);
			printf(HELP_STRING);
			exit(0);
		}

		else if (check_arg_equals(argv[i], "-v", "--version", NULL)) {
			printf(VERSION_STRING);
			exit(0);
		}

		else if (check_arg_equals(argv[i], "-s", "--alt-settings", NULL)) {
			if (i + 1 < argc) {
				strcpy(alt_settings_file, argv[i + 1]);
			}
			else {
				printf("Err - '--alt-settings' flag must provide a string pointing to the alternate settings file.\n");
				exit(1);
			}
		}
	}
}

void process_json_config() {
	char buffer[MAX_JSON_FILE_SIZE];
	FILE *file_ptr = fopen(alt_settings_file, "r");
	char line_items[MAX_TOKEN_SIZE][MAX_JSON_KEYS];

	if (file_ptr) {
		fread(buffer, MAX_JSON_FILE_SIZE, 1, file_ptr);
		fclose(file_ptr);
	}
	else {
		fprintf(stderr, "Err - file can't be opened: '%s' \n", alt_settings_file);
		exit(1);
	}

	cJSON *data_json = cJSON_Parse(buffer);
	cJSON *meta_editor_json = cJSON_GetObjectItemCaseSensitive(data_json, "meta_editor");
	if (cJSON_IsString(meta_editor_json))
		meta_editor = strdup(meta_editor_json->valuestring);

	cJSON *default_header_location_json = cJSON_GetObjectItemCaseSensitive(data_json, "default_header_location");
	if (cJSON_IsString(default_header_location_json))
		default_header_location = strdup(default_header_location_json->valuestring);

	cJSON *use_wine_json = cJSON_GetObjectItemCaseSensitive(data_json, "use_wine");
	if (cJSON_IsBool(use_wine_json))
		use_wine = use_wine_json->valueint;

	cJSON *compiler_colors_json = cJSON_GetObjectItemCaseSensitive(data_json, "compiler_colors");
	cJSON *compiler_colours_json = cJSON_GetObjectItemCaseSensitive(data_json, "compiler_colours");
	if (cJSON_IsBool(compiler_colors_json))
		colourful = compiler_colors_json->valueint;
	else if (cJSON_IsBool(compiler_colours_json))
		colourful = compiler_colours_json->valueint;

	cJSON *suppress_launch_errors_json = cJSON_GetObjectItemCaseSensitive(data_json, "suppress_errors");
	if (cJSON_IsBool(suppress_launch_errors_json))
		suppress_launch_errors = suppress_launch_errors_json->valueint;

	cJSON *use_path_json = cJSON_GetObjectItemCaseSensitive(data_json, "use_path");
	cJSON *use_PATH_json = cJSON_GetObjectItemCaseSensitive(data_json, "use_PATH");
	if (cJSON_IsBool(use_path_json))
		use_PATH = use_path_json->valueint;
	else if (cJSON_IsBool(use_PATH_json))
		use_PATH = use_PATH_json->valueint;

	cJSON_Delete(data_json);
}

void process_command_flags(int argc, char *argv[]) {
	int arg_num = 0;
	if (argc > 1 && is_directory(argv[1])) {
		work_area = argv[1];
		arg_num++;
	}

	for (; arg_num < argc; arg_num++) {
		if (check_arg_equals(argv[arg_num], "-me", "--meta-editor", NULL))
			meta_editor = argv[arg_num + 1];

		else if (check_arg_equals(argv[arg_num], "-dh", "--default-headers", NULL))
			default_header_location = argv[arg_num + 1];

		else if (check_arg_equals(argv[arg_num], "-wine", "--use-wine", NULL)) {
			if (argc - 1 > arg_num && argv[arg_num + 1][0] == '-' || argc - 1 == arg_num) {
				use_wine = true;
			}
			else {
				int arg_elem = string_to_bool(string_lower(argv[arg_num + 1]));
				if (arg_elem == 0) use_wine = false;
				else if (arg_elem == 1) use_wine = true;
				else {
					fprintf(stderr, "Err - failed to interpret flag %s's value: %s\n", argv[arg_num], argv[arg_num + 1]);
					exit(1);
				}
			}
		}

		else if (check_arg_equals(argv[arg_num], "-clr", "--colourful", NULL)) {
			if (argc - 1 > arg_num && argv[arg_num + 1][0] == '-' || argc - 1 == arg_num) {
				colourful = true;
			}
			else {
				int arg_elem = string_to_bool(string_lower(argv[arg_num + 1]));
				if (arg_elem == 0) colourful = false;
				else if (arg_elem == 1) colourful = true;
				else {
					fprintf(stderr, "Err - failed to interpret flag %s's value: %s\n", argv[arg_num], argv[arg_num + 1]);
					exit(1);
				}
			}
		}

		else if (check_arg_equals(argv[arg_num], "-se", "--suppress-errors", NULL)) {
			if (argc - 1 > arg_num && argv[arg_num + 1][0] == '-' || argc - 1 == arg_num) {
				suppress_launch_errors = true;
			}
			else {
				int arg_elem = string_to_bool(string_lower(argv[arg_num + 1]));
				if (arg_elem == 0) suppress_launch_errors = false;
				else if (arg_elem == 1) suppress_launch_errors = true;
				else {
					fprintf(stderr, "Err - failed to interpret flag %s's value: %s\n", argv[arg_num], argv[arg_num + 1]);
					exit(1);
				}
			}
		}

		else if (check_arg_equals(argv[arg_num], "-path", "--use-path", NULL)) {
			if (argc - 1 > arg_num && argv[arg_num + 1][0] == '-' || argc - 1 == arg_num) {
				use_PATH = true;
			}
			else {
				int arg_elem = string_to_bool(string_lower(argv[arg_num + 1]));
				if (arg_elem == 0) use_PATH = false;
				else if (arg_elem == 1) use_PATH = true;
				else {
					fprintf(stderr, "Err - failed to interpret flag %s's value: %s\n", argv[arg_num], argv[arg_num + 1]);
					exit(1);
				}
			}
		}

		else if (argv[arg_num][0] == '-' && arg_num > 0) {
			fprintf(stderr, "Err - Unknown flag '%s', helpfile:\n\n", argv[arg_num]);
			printf(VERSION_STRING);
			printf(HELP_STRING);
			exit(1);
		}
	}
}

