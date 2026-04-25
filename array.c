#include "array.h"
#include "globals.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void free_array(array **target) {
	if (target == NULL || *target == NULL)
		return;

	char *elem;
	int i = 0;
	ARRAY_FOREACH(elem, *target, i)
		free(elem);
	free((*target)->array);
	free(*target);
	*target = NULL;
}

int array_append(array **dest, array **src) {
	array *res = NEW_ARRAY(MAX_ARRAY_SIZE);
	if (*dest == NULL || *src == NULL || src == NULL || dest == NULL || (*dest)->count + (*src)->count > MAX_ARRAY_SIZE || (*dest)->count + (*src)->count == 0) {
		free_array(&res);
		return -1;
	}

	char *elem;
	int i = 0;

	ARRAY_FOREACH(elem, *dest, i)
		res->array[res->count++] = strdup(elem);

	i = 0;
	ARRAY_FOREACH(elem, *src, i)
		res->array[res->count++] = strdup(elem);

	free_array(dest);
	free_array(src);

	*dest = res;
	return 0;
}
