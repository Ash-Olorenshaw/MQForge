#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>

#include "globals.h"



char *string_lower(char *str) {
	for (int i = 0; str[i]; i++) {
		str[i] = tolower(str[i]);
	}
	return str;
}

bool string_isspace(char *str) {
	for (int i = 0; i < strlen(str); i++) {
		if (!isspace(str[i]))
			return false;
	}
	return true;
}

char *replace_str_ending(char *str, char *replacement_str) {
	for (int i = 0; i < strlen(replacement_str); i++) {
		str[strlen(str) - 1 - i] = replacement_str[strlen(replacement_str) - 1 - i];
	}
	return str;
}

int string_to_bool(char *item) {
	if (strcmp(item, "true") == 0)
		return 1;
	else if (strcmp(item, "false") == 0)
		return 0;
	else
		return -1;
}

char *ltrim_char(char *str, char target_char) {
	while(*str == target_char) str++;
	return str;
}

char *rtrim_char(char *str, char target_char) {
	char* back = str + strlen(str) - 1;
	while(*back == target_char || isspace(*back) || *back == 0){
		back--;
	}
	*(back + 1) = '\0';
	return str;
}

bool check_arg_equals(const char *flags, ...) {
	va_list args;
	const char *flag;
	va_start(args, flags);
	const char *target_flag = flags;

	while ((flag = va_arg(args, const char *)) != NULL) {
		if (strcmp(target_flag, flag) == 0) {
			return true;
		}
	}
	va_end(args);
	return false;
}


char *extract_delimited_string(char *input, const char target_char) {
	int length = strlen(input);

	if (input[0] == target_char && input[length - 1] == target_char) {
		for (int i = 1; i < length - 1; i++) {
			input[i - 1] = input[i];
		}
		input[length - 2] = '\0';
	}

	return input;
}

bool file_exists(char *filename) {
	struct stat buffer;   
	return stat(filename, &buffer) == 0;
}

char *trim_char(char *str, char target_char) {
	char *ltrimmed = ltrim_char(str, target_char);
	char *rtrimmed = rtrim_char(ltrimmed, target_char);
	return rtrimmed; 
}

char *ltrim(char *str) {
	while(isspace(*str)) str++;
	return str;
}

char *rtrim(char *str) {
	char* back = str + strlen(str);
	while(isspace(*--back));
	*(back + 1) = '\0';
	return str;
}

char *trim(char *str) {
	return rtrim(ltrim(str)); 
}

int string_occurences(char *str, char target_char) {
	int i, count;
	for (i=0, count=0; str[i]; i++)
		count += (str[i] == target_char);
	return count;
}

char *get_substring(char *str, int pos, int l, char *new_str) {
	int i = 0;
	int str_len = strlen(str);

	while (i < l && i < str_len) {
		new_str[i] = str[pos + i];
		i++;
	}
	new_str[i] = '\0';  

	return new_str;
}

bool value_in_string_array(char val[MAX_TOKEN_SIZE], char arr[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE], int n) {
	for (int i = 0; i < n; i++) {
		if (strcmp(arr[i], val) == 0)
			return true;
	}
	return false;
}

int split_string(char *string_item, char delimiter, char tokens_out[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE]) {
	char current_string[MAX_TOKEN_SIZE] = "";
	int count = 0;

	for (int i = 0; i < strlen(string_item); i++) {
		char ch = string_item[i];
		
		if (ch == delimiter) {
			strncpy(tokens_out[count], current_string, MAX_TOKEN_SIZE);
			count++;
			memset(current_string, 0, sizeof(current_string));
		}
		else if (i == (strlen(string_item) - 1)) {
			if (strlen(current_string) > MAX_TOKEN_SIZE - 1) {
				printf("Err - String %s exceeded expected token size of %d", string_item, MAX_TOKEN_SIZE);
				return 400;
			}
			current_string[strlen(current_string)] = ch;
			current_string[strlen(current_string) + 1] = '\0';
			strncpy(tokens_out[count], current_string, MAX_TOKEN_SIZE);
			count++;
			memset(current_string, 0, sizeof(current_string));
		}
		else {
			current_string[strlen(current_string)] = ch;
			current_string[strlen(current_string) + 1] = '\0';
		}
	}
	return count;
}

char *split_get_second_half(char *str, const char *delim) {
	char *p = strstr(str, delim);

	if (p == NULL) return NULL;     // delimiter not found

	*p = '\0';                      // terminate string after head
	return p + strlen(delim);       // return tail substring
}


char *split_get_first_half(char *str, const char *delim) {
	char *p = strstr(str, delim);
	if (p == NULL) return NULL;

	*p = '\0';
	return str;
}
