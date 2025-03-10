#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "basic_json_parse.h"
#include "file_utils.h"
#include "utils.h"
#include "globals.h"
#include "file_orderer.h"
#include "compiler.h"

#define HELP_STRING "\nOPTIONAL:\n[<filepath>]\n\nREQUIRED:\n[-me/--meta-editor]\t-\tLocation for your metaeditor.exe file.\n\nOPTIONAL:\n[-h/--help]\t\t-\tprint this helpfile\n[-v/--version]\t\t-\tprint program version\n[-dh/--default-headers]\t-\tLocation directory of standard .mqh header files.\n[-wine/--use-wine]\t-\t(default: false) Whether to use Wine to run 'metaeditor.exe' - only available on Linux.\n[-clr/--colourful]\t-\t(default: true) Whether to provide a coloured output.\n[-se/--suppress-errors]\t-\t(default: false) Whether to suppress launch errors for metaeditor.exe (really only matters with Wine).\n[-path/--use-path]\t-\t(default: true) Whether to search your PATH for .ex4, .dll, and .mqh files.\n[-s/--alt-settings]\t-\tAlternate settings file as opposed to the default 'compiler_commands.json' file.\n\n"
#define VERSION_STRING "MQForge v0.0.1\n"

#ifdef _WIN32
#include "sys_interactions_windows.h"
#else
#include "sys_interactions_linux.h"
#endif
// cmake -DTARGET_PLATFORM=Linux -B build -S . && cd build && make && cd ..

int main(int argc, char *argv[]) {
	for (int i = 0; i < argc; i++) {
		if (check_arg_equals(argv[i], "-h", "--help", NULL)) {
			printf(VERSION_STRING);
			printf(HELP_STRING);
			return 0;
		}

		else if (check_arg_equals(argv[i], "-v", "--version", NULL)) {
			printf(VERSION_STRING);
			return 0;
		}

		else if (check_arg_equals(argv[i], "-s", "--alt-settings", NULL)) {
			strcpy(alt_settings_file, argv[i + 1]);
		}
	}

	int arg_num = 0;
	if (argc > 1 && is_directory(argv[1])) {
		work_area = argv[1];
		arg_num++;
	}

	int json_size = 0;
	struct mapElement json[MAX_JSON_KEYS];

	read_basic_json(alt_settings_file, &json_size, &json);
	for (int i = 0; i < json_size; i++) {
		char *key = json[i].key;
		if (strcmp("meta_editor", key) == 0) {
			if (string_to_bool(json[i].val) < 0) {
				extract_delimited_string(meta_editor = json[i].val, '"');
			}
		}
		else if (strcmp("default_header_location", key) == 0) {
			if (string_to_bool(json[i].val) < 0) {
				extract_delimited_string(default_header_location = json[i].val, '"');
			}
		}
		else if (strcmp("use_wine", key) == 0) {
			if (string_to_bool(json[i].val) > -1) {
				use_wine = string_to_bool(json[i].val);
			}
		}
		else if (strcmp("compiler_colours", key) == 0 || strcmp("compiler_colors", key) == 0) {
			if (string_to_bool(json[i].val) > -1) {
				colourful = string_to_bool(json[i].val);
			}
		}
		else if (strcmp("suppress_errors", key) == 0) {
			if (string_to_bool(json[i].val) > -1) {
				suppress_launch_errors = string_to_bool(json[i].val);
			}
		}
		else if (strcmp("use_path", key) == 0 || strcmp("use_PATH", key) == 0) {
			if (string_to_bool(json[i].val) > -1) {
				use_PATH = string_to_bool(json[i].val);
			}
		}
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

	if (meta_editor == NULL || strcmp(meta_editor, "") == 0) {
		fprintf(stderr, "Err - you must include a 'metaeditor.exe' location to compile. Either use flags -me/--meta-editor or key 'meta_editor' in forge_commands.json\n");
		exit(1);
	}

	if ((strcmp(OS_NAME, "Linux") == 0 && alt_settings_file[0] != '/') || (strcmp(OS_NAME, "Windows") == 0 && alt_settings_file[1] != ':')) {
		char relative_settings_file[MAX_TOKEN_SIZE];
		strcpy(relative_settings_file, work_area);
		strcat(relative_settings_file, "/");
		strcat(relative_settings_file, alt_settings_file);
		strcpy(alt_settings_file, relative_settings_file);
	}

	printf("Starting compile in %s with:\n", work_area);
	printf("\t- meta_editor = '%s'\n", meta_editor);
	printf("\t- default_header_location = '%s'\n", default_header_location);
	printf("\t- use_wine = %s\n", use_wine ? "true" : "false");
	printf("\t- colourful = %s\n", colourful ? "true" : "false");
	printf("\t- suppress_launch_errors = %s\n", suppress_launch_errors ? "true" : "false");
	printf("\t- use_PATH = %s\n", use_PATH ? "true" : "false");

	char meta_quotes_files[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE] = {0};
	int meta_quotes_file_count = search_dir_for_ext(work_area, "mq4", meta_quotes_files);

	char additional_search_dirs[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE] = {0};
	int dlls_found = search_PATH_for_ext("dll", additional_search_dirs, available_dlls);

	char default_header_location_arr[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE];
	strcpy(default_header_location_arr[0], default_header_location);
	int headers_count = search_PATH_for_ext("mqh", default_header_location_arr, available_headers);

	char ordered_files[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE] = {0};
	int ordered_file_count = 0;

	int success = create_file_order(meta_quotes_files, ordered_files, &ordered_file_count);
	compile_files(ordered_files, ordered_file_count);
	if (file_exists("errors.log")) {
		remove("errors.log");
	}

	return 0;
}

