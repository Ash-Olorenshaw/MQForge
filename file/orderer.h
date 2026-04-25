#ifndef __FILE_ORDERER_H
#define __FILE_ORDERER_H

#include "../array.h"

int create_file_order(array *files, array **dependency_tree, int *file_num, array * available_dlls, array *available_headers);

#endif
