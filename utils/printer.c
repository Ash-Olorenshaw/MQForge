#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "../globals.h"

bool running = false;

void spinner_stop() {
	running = false;
}

void* spinner_thread(void* arg) {
	running = true;
	int state = 0;

	while (running) {
		printf("\r");
		switch (state) {
			case 0:
				printf("⠋");
				break;
			case 1:
				printf("⠙");
				break;
			case 2:
				printf("⠹");
				break;
			case 3:
				printf("⠸");
				break;
			case 4:
				printf("⠼");
				break;
			case 5:
				printf("⠴");
				break;
			case 6:
				printf("⠦");
				break;
			case 7:
				printf("⠧");
				break;
			case 8:
				printf("⠇");
				break;
			case 9:
				printf("⠏");
				break;

		}
		state += 1;
		if (state > 9)
			state = 0;
		usleep(100000);
		fflush(stdout);
	}

	printf("\r \n");
    return NULL;
}

void print_run_info() {
	printf("Starting compile in %s with:\n", work_area);
	printf("\t- meta_editor = '%s'\n", meta_editor);
	printf("\t- default_header_location = '%s'\n", default_header_location);
	printf("\t- use_wine = %s\n", use_wine ? "true" : "false");
	printf("\t- colourful = %s\n", colourful ? "true" : "false");
	printf("\t- suppress_launch_errors = %s\n", suppress_launch_errors ? "true" : "false");
	printf("\t- use_PATH = %s\n", use_PATH ? "true" : "false");
}

