#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include "globals.h"

char *string_lower(char *str);

int string_to_bool(const char *item);
bool check_arg_equals(const char *flags, ...);
bool file_exists(char *filename);

char *ltrim_char(char *str, char target_char);
char *rtrim_char(char *str, char target_char);
char *trim_char(char *str, char target_char);

char *extract_delimited_string(char *str, const char target_char);

char *ltrim(char *str);
char *rtrim(char *str);
char *trim(char *str);

bool value_in_string_array(char val[MAX_TOKEN_SIZE], char arr[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE], int n);

int string_occurences(char *str, char target_char);
int split_string(char *string_item, char delimiter, char tokens_out[MAX_ARRAY_SIZE][MAX_TOKEN_SIZE]);
char *get_substring(char *str, int pos, int l, char *new_str);
bool string_isspace(char *str);
char *replace_str_ending(char *str, char *replacement_str);

char *split_get_second_half(char *str, const char *delim);
char *split_get_first_half(char *str, const char *delim);

#endif
