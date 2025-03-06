#include <stdbool.h>
#include "globals.h"

char available_dlls[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE] = {0};
char available_headers[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE] = {0};
char *market_platform = "";
char *meta_editor = "";
char *default_header_location = "";
bool use_wine = false;
bool colourful = true;
bool suppress_launch_errors = false;
bool use_PATH = true;
char alt_settings_file[MAX_TOKEN_SIZE] = "forge_commands.json";
char *work_area = ".";
struct map_element_array dependency_dict[MAX_ARRAY_SIZE] = { 0 };

