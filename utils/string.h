#ifndef __UTILS_STRING_H
#define __UTILS_STRING_H

#include <stdbool.h>
#include "../globals.h"

char *string_lower(char *str);

int string_to_bool(const char *item);
bool check_arg_equals(const char *flags, ...);

char *ltrim_char(char *str, char target_char);
char *rtrim_char(char *str, char target_char);
char *trim_char(char *str, char target_char);

char *extract_delimited_string(char *str, const char target_char);

char *ltrim(char *str);
char *rtrim(char *str);
char *trim(char *str);

bool value_in_string_array(char val[MAX_TOKEN_SIZE], array *arr, int n);

int string_occurences(char *str, char target_char);
int split_string(char *string_item, char delimiter, array *tokens_out);
char *get_substring(const char *str, int start, int end);
bool string_isspace(char *str);
char *replace_str_ending(char *str, char *replacement_str);

char *split_get_second_half(char *str, const char *delim);
char *split_get_first_half(char *str, const char *delim);

#endif
