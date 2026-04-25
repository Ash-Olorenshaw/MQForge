#include <stdbool.h>
#include <stdlib.h>

#include "globals.h"

char *market_platform = "";
char *meta_editor = "";
char *default_header_location = "";
bool use_wine = false;
bool colourful = true;
bool suppress_launch_errors = false;
bool use_PATH = true;
char alt_settings_file[MAX_TOKEN_SIZE] = "forge_commands.json";
char *work_area = ".";
array *target_files = NULL;

