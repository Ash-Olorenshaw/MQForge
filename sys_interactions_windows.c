#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

#include "utils.h"
#include "globals.h"

#define OS_NAME "Windows"
#define PATH_SIZE 1000

int get_PATH(char final_PATH_elems[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE]) {
	FILE *p;
	int ch;
	char final_PATH[MAX_PATH_LEN] = "";

	p = popen("echo %PATH%","r");   
	if( p == NULL) {
		puts("Unable to open $PATH process... skipping...");
		return 0;
	}
	while((ch = fgetc(p)) != EOF) {
		if (strlen(final_PATH) < (MAX_PATH_LEN - 1)) {
			final_PATH[strlen(final_PATH)] = ch;
		}
	}
	final_PATH[strlen(final_PATH) + 1] = '\0';
	pclose(p);

	split_string(final_PATH, ';', final_PATH_elems);

	return 1;
}
