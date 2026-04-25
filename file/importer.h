#ifndef __FILE_IMPORTER_H
#define __FILE_IMPORTER_H

#include "../globals.h"

int check_file_deps(
	char file[MAX_TOKEN_SIZE],
	array *import_items,
	array *available_dlls,
	array *available_headers
);

#endif
