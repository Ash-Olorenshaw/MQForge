#ifndef __COMPILER_H
#define __COMPILER_H

#include "array.h"

typedef enum {
	NONE,
	RED,
	ORANGE,
	CYAN,
	GREEN,
} PRINT_COLOR;

void compile_files(array *target_files);

#endif
