#ifndef FILE_ORDERER_H
#define FILE_ORDERER_H

#include "globals.h"

int create_file_order(char files[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE], char dependency_tree[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE], int *file_num);

#endif
