#include <stdbool.h>

#define MAX_TOKEN_SIZE 1024
#define MAX_ARRAY_SIZE 500
#define MAX_PATH_LEN 1024
#define MAX_JSON_KEYS 256
#define MAX_JSON_FILE_SIZE 4096

struct map_element_array {
	char *key;
	char val[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE];
};

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
struct map_element_array dependency_dict[MAX_ARRAY_SIZE] = {0};

