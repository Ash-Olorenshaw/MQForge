#if 0
	printf "BUILDING SCRIPT...\n"
    gcc "$0" \
		./build_scripts/utils.c ./build_scripts/args.c \
		-o ./.temp-run \
		-Wall -Wextra -Wno-gnu -pedantic
	printf "\n"
	./.temp-run "$@"
    rm -f ./.temp-run
    exit
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "build_scripts/utils.h"
#include "build_scripts/args.h"

#define BUILD_FILES "main.c", \
	"utils/printer.c", \
	"utils/string.c", \
	"file/utils.c", \
	"file/importer.c", \
	"file/orderer.c", \
	"file/dependencies.c", \
	"file/main.c", \
	"array.c", \
	"globals.c", \
	"cJSON.c", \
	"config.c", \
	"compiler.c", \
	"sys_interactions_linux.c"
#define OUTPUT_FILE "./MQForge"
#define BUILD_ARGS "-Wall", "-Wextra", "-pedantic"

int main(int argv, const char **argc) {
	args arguments = { .arg_count = argv, .args = argc };
	char *output_file_name = arg_pos("windows", arguments) == -1 ? OUTPUT_FILE : (OUTPUT_FILE".exe");

	char *compiler;
	if (argv < 2 || strcmp(argc[1], "-") == 0) {
		printf("No compiler provided, defaulting to `gcc`\n");
		compiler = "gcc";
	}
	else
		compiler = (char *) argc[1];

	char** debug_args = (char**) ARR_CREATE("-g", "-fsanitize=address", "-fno-omit-frame-pointer" );
	char **build_args = (char**) ARR_CREATE(
		compiler,
		BUILD_FILES,
		BUILD_ARGS,
		"-o",
		output_file_name
	);

	if (arg_pos("debug", arguments) != -1) {
		char **new_build_args = (char **) array_cat((void**) build_args, (void**) debug_args);
		free(build_args);
		build_args = new_build_args;

	}

	int time = run_command(build_args, ".", false);
	printf("Build finished in %d seconds. DEBUG: '%s'\n", time, arg_pos("debug", arguments) != -1 ? "true" : "false");
	free(build_args);
	free(debug_args);

	if (arg_pos("run", arguments) != -1) {
		char *run_args[] = { output_file_name, NULL };
		run_command(run_args, ".", false);
	}
}

