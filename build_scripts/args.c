#include <string.h>
#include <stdbool.h>
#include "./args.h"

int arg_pos(const char *arg, args arguments) {
	for (int i = 0; i < arguments.arg_count; i++) {
		if (strcmp(arguments.args[i], arg) == 0) {
			return i;
		}
	}
	return -1;
}

