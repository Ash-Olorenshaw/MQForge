#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "utils.h"

double timer_start_val, timer_stop_val;

bool command_exists(const char *command) {
	char *sys_command;
	bool result;

	sys_command = (char*) malloc(sizeof(char) * (strlen(command) + 24));
	strcpy(sys_command, "which ");
	strcat(sys_command, command);
	strcat(sys_command, " > /dev/null 2>&1");

	result = !system(sys_command);
	free(sys_command);

	return result;
}

char *str_join(char *const str_array[]) {
	int i = 0, result_len = 0;
	while (str_array[i] != NULL)
		result_len += strlen(str_array[i++]);
	result_len += i; // add in spaces and \0

	char *result = (char *) malloc(sizeof(char) * result_len);
	strcpy(result, str_array[0]);
	i = 1;

	while (str_array[i] != NULL) {
		strcat(result, " ");
		strcat(result, str_array[i++]);
	}

	result[result_len - 1] = '\0';
	return result;
}

double run_command(char *const args[], const char *dir, bool no_stdout) {
	chdir(dir);
	pid_t pid = fork();

	if (pid == -1) {
		printf("Error with forking new process.\n");
		exit(1);
	}
	else if (pid > 0) {
		int status;
		START_TIME;
		waitpid(pid, &status, 0);
		END_TIME;
		return GET_TIME;
	}
	else {
		if (no_stdout) {
			int devnull = open("/dev/null", O_WRONLY);
			dup2(devnull, STDOUT_FILENO);
			close(devnull);
		}

		if (execvp(args[0], args) == -1) {
			char *arg_str = str_join(args);
			fprintf(stderr, "Error with running command: \n\t%s\n", arg_str);
			free(arg_str);
			perror(NULL);
			exit(1);
		}
	}
	return 0;
}

