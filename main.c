#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <pthread.h>

#include "file/utils.h"
#include "globals.h"
#include "file/orderer.h"
#include "compiler.h"
#include "utils/printer.h"
#include "utils/string.h"
#include "cJSON.h"
#include "config.h"

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
	process_json_config(data_json);
	process_command_flags(argc, argv);

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

	print_run_info();
	printf("Searching PATH for relevant files...\n");
	pthread_t spinner_tid;
    pthread_create(&spinner_tid, NULL, spinner_thread, NULL);

		char meta_quotes_files[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE] = {0};
		int meta_quotes_file_count = search_dir_for_ext(work_area, "mq4", meta_quotes_files, true);

		char additional_search_dirs[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE] = {0};
		int dlls_found = search_PATH_for_ext("dll", additional_search_dirs, available_dlls);

		char default_header_location_arr[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE];
		strcpy(default_header_location_arr[0], default_header_location);
		int headers_count = search_PATH_for_ext("mqh", default_header_location_arr, available_headers);

	spinner_stop();
	pthread_join(spinner_tid, NULL);
	printf("Files found.\n");

	char ordered_files[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE] = {0};
	int ordered_file_count = 0;

	int success = create_file_order(meta_quotes_files, ordered_files, &ordered_file_count);
	compile_files(ordered_files, ordered_file_count);
	if (file_exists("errors.log"))
		remove("errors.log");

	cJSON_Delete(data_json);
	return 0;
}

