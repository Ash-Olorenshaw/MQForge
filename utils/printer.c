#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

bool running = false;

void spinner_stop() {
	running = false;
}

void* spinner_thread(void* arg) {
	running = true;
	int state = 0;

	printf("\n");
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

	printf("\n");
    return NULL;
}

