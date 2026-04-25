#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file/utils.h"
#include "file/main.h"
#include "utils/printer.h"
#include "globals.h"
#include "compiler.h"
#include "config.h"

#ifdef _WIN32
#include "sys_interactions_windows.h"
#else
#include "sys_interactions_linux.h"
#endif

int main(int argc, char *argv[]) {
	process_core_flags(argc, argv);
	process_json_config();
	process_command_flags(argc, argv);

	if (meta_editor == NULL || strcmp(meta_editor, "") == 0) {
		fprintf(stderr, "Err - you must include a 'metaeditor.exe' location to compile. Either use flags -me/--meta-editor or key 'meta_editor' in forge_commands.json\n");
		exit(1);
	}

	if ((strcmp(OS_NAME, "Linux") == 0 && alt_settings_file[0] != '/') || (strcmp(OS_NAME, "Windows") == 0 && alt_settings_file[1] != ':')) {
		char *relative_settings_file = malloc(strlen(work_area) + 1 + strlen(alt_settings_file) + 1);
		strcpy(relative_settings_file, work_area);
		strcat(relative_settings_file, "/");
		strcat(relative_settings_file, alt_settings_file);
		strcpy(alt_settings_file, relative_settings_file);
		free(relative_settings_file);
	}

	print_run_info();

	array *ordered_files = NEW_ARRAY(MAX_ARRAY_SIZE);
	compile_files(assemble_compile_order(&ordered_files));

	if (file_exists("errors.log"))
		remove("errors.log");
	free_array(&ordered_files);

	return 0;
}

