#ifndef __GLOBALS_H
#define __GLOBALS_H

#include <stdbool.h>

#define MAX_TOKEN_SIZE 256
#define MAX_ARRAY_SIZE 500
#define MAX_PATH_LEN 1024
#define MAX_JSON_KEYS 256
#define MAX_JSON_FILE_SIZE 4096
#define HELP_STRING "\nOPTIONAL:\n[<filepath>]\n\nREQUIRED:\n[-me/--meta-editor]\t-\tLocation for your metaeditor.exe file.\n\nOPTIONAL:\n[-h/--help]\t\t-\tprint this helpfile\n[-v/--version]\t\t-\tprint program version\n[-dh/--default-headers]\t-\tLocation directory of standard .mqh header files.\n[-wine/--use-wine]\t-\t(default: false) Whether to use Wine to run 'metaeditor.exe' - only available on Linux.\n[-clr/--colourful]\t-\t(default: true) Whether to provide a coloured output.\n[-se/--suppress-errors]\t-\t(default: false) Whether to suppress launch errors for metaeditor.exe (really only matters with Wine).\n[-path/--use-path]\t-\t(default: true) Whether to search your PATH for .ex4, .dll, and .mqh files.\n[-s/--alt-settings]\t-\tAlternate settings file as opposed to the default 'compiler_commands.json' file.\n\n"
#define VERSION_STRING "MQForge v0.0.1\n"

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
extern char alt_settings_file[MAX_TOKEN_SIZE];
extern char *work_area;
extern struct map_element_array dependency_dict[MAX_ARRAY_SIZE];

#endif
