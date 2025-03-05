#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <dirent.h>

#include "utils.h"
#include "globals.h"

#define OS_NAME "Linux"


int get_PATH(char final_PATH_elems[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE]) {
	FILE *p;
	int ch;
	char final_PATH[MAX_PATH_LEN] = "";

	p = popen("echo $PATH","r");   
	if (p == NULL) {
		fprintf(stderr, "Err - Unable to open $PATH... skipping...\n");
		return 0;
	}
	while((ch = fgetc(p)) != EOF) {
		if (strlen(final_PATH) < (MAX_PATH_LEN - 1)) {
			final_PATH[strlen(final_PATH)] = ch;
		}
	}
	final_PATH[strlen(final_PATH) + 1] = '\0';

	pclose(p);
	
	split_string(final_PATH, ':', final_PATH_elems);

	return 1;
}

