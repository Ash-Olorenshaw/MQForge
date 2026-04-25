#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "utils.h"
#include "orderer.h"
#include "../utils/printer.h"
#include "../globals.h"

array *assemble_compile_order(array **ordered_files) {
	array *available_dlls = NEW_ARRAY(MAX_ARRAY_SIZE);
	array *available_headers = NEW_ARRAY(MAX_ARRAY_SIZE);
	array *meta_quotes_files = NEW_ARRAY(MAX_ARRAY_SIZE);
	array *additional_search_dirs = NEW_ARRAY(MAX_ARRAY_SIZE);
	array *default_header_location_arr = NEW_ARRAY(MAX_ARRAY_SIZE);

	default_header_location_arr->array[default_header_location_arr->count++] = strdup(default_header_location);

	printf("Searching PATH for relevant files...\n");
	pthread_t spinner_tid;
    pthread_create(&spinner_tid, NULL, spinner_thread, NULL);

	if (target_files == NULL)
		search_dir_for_ext(work_area, "mq4", meta_quotes_files, true);
	else {
		free_array(&meta_quotes_files);
		meta_quotes_files = target_files;
	}

	search_PATH_for_ext("dll", additional_search_dirs, &available_dlls);
	search_PATH_for_ext("mqh", default_header_location_arr, &available_headers);

	spinner_stop();
	pthread_join(spinner_tid, NULL);
	printf("All relevant files discovered. (%d)\n", meta_quotes_files->count);

	int ordered_file_count;

	create_file_order(meta_quotes_files, ordered_files, &ordered_file_count, available_dlls, available_headers);
	free_array(&default_header_location_arr);
	free_array(&additional_search_dirs);
	free_array(&meta_quotes_files);
	free_array(&available_headers);
	free_array(&available_dlls);

	return *ordered_files;
}
