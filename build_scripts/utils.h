#ifndef BUILD_SCRIPTS_UTILS_H
#define BUILD_SCRIPTS_UTILS_H

#include <stdbool.h>

extern double timer_start_val, timer_stop_val;

#define GET_TIME timer_stop_val - timer_start_val

#define START_TIME { \
	struct timeval t; gettimeofday(&t, NULL); \
	timer_start_val = t.tv_sec + t.tv_usec/1000000.0; \
}

#define END_TIME { \
	struct timeval t; gettimeofday(&t, NULL); \
	timer_stop_val = t.tv_sec + t.tv_usec/1000000.0; \
}

bool command_exists(const char *command);
char *str_join(char *const str_array[]);
double run_command(char *const args[], const char *dir, bool no_stdout);

#endif

