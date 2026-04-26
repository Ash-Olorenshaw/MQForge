#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

#include "../globals.h"

char *string_lower(char *str) {
	for (int i = 0; str[i]; i++) {
		str[i] = tolower(str[i]);
	}
	return str;
}

bool string_isspace(char *str) {
	for (int i = 0; i < (int) strlen(str); i++) {
		if (!isspace(str[i]))
			return false;
	}
	return true;
}

char *replace_str_ending(char *str, char *replacement_str) {
	for (int i = 0; i < (int) strlen(replacement_str); i++) {
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
	if (strlen(str) == 0)
		return str;
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

char *get_substring(const char *str, int start, int end) {
	if (str == NULL || start < 0 || end < 0 || start >= end)
		return NULL;

	int len = strlen(str);

	if (start >= len) return strdup("");
	if (end > len) end = len;

	int sublen = end - start;
    char *result = malloc(sublen + 1);

    if (!result) return NULL;

    memcpy(result, str + start, sublen);
    result[sublen] = '\0';
	return result;
}

bool value_in_string_array(char val[MAX_TOKEN_SIZE], array * arr, int n) {
	for (int i = 0; i < n; i++) {
		if (strcmp(arr->array[i], val) == 0 && i < arr->size)
			return true;
	}
	return false;
}

int split_string(char *string_item, char delimiter, array *tokens_out) {
	if (string_item == NULL) {
		tokens_out->array[0] = NULL;
		return 0;
	}

	char current_string[MAX_TOKEN_SIZE] = "";
	int string_len = strlen(string_item);

	for (int i = 0; i < string_len && tokens_out->count < tokens_out->size; i++) {
		char ch = string_item[i];

		if (ch == delimiter) {
			tokens_out->array[tokens_out->count++] = strdup(current_string);
			memset(current_string, 0, sizeof(current_string));
		}
		else if (i == (string_len - 1)) {
			if (strlen(current_string) > MAX_TOKEN_SIZE - 1) {
				fprintf(stderr, "Err - String %s exceeded expected token size of %d when splitting...\n", string_item, MAX_TOKEN_SIZE);
				return 400;
			}
			current_string[strlen(current_string)] = ch;
			current_string[strlen(current_string) + 1] = '\0';
			tokens_out->array[tokens_out->count++] = strdup(current_string);
			memset(current_string, 0, sizeof(current_string));
		}
		else {
			current_string[strlen(current_string)] = ch;
			current_string[strlen(current_string) + 1] = '\0';
		}
	}
	return tokens_out->count;
}

char *split_get_second_half(char *str, const char *delim) {
	char *p = strstr(str, delim);
	if (p == NULL) return NULL;

	*p = '\0';
	return p + strlen(delim);
}


char *split_get_first_half(char *str, const char *delim) {
	char *p = strstr(str, delim);
	if (p == NULL) return NULL;

	*p = '\0';
	return str;
}
