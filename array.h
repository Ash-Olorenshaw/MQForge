#ifndef __ARRAY_H
#define __ARRAY_H

#include <stddef.h>

// #define ARRAY_FOREACH(elem, target, i)  \
// 	for (elem = (target != NULL) ? (target)->array[i++] : NULL; i < (target)->count && elem != NULL; elem = (target)->array[i++])
#define ARRAY_FOREACH(elem, target, i) \
    for (i = 0; (target) != NULL && i < (target)->count && ((elem) = (target)->array[i]) != NULL; i++)

#define NEW_ARRAY(target_size) ({ \
		array *res = malloc(sizeof(array)); \
		res->array = malloc(target_size * sizeof(void *)); \
		res->size = target_size; \
		res->count = 0; \
		for (int i = 0; i < res->size; i++) \
			res->array[i] = NULL; \
		res; \
	}) \

typedef struct {
	void **array;
	int count;
	int size;
} array;

void free_array(array **target);
int array_append(array **dest, array **src);

#endif
