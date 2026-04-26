#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>
#include <stdint.h>

#include "utils/string.h"
#include "file/utils.h"
#include "compiler.h"

#define BOM_UTF16_LE 0xFFFE
#define BOM_UTF16_BE 0xFEFF


int read_utf16_char_file(FILE *file, uint16_t *ch) {
	uint8_t bytes[2];
	if (fread(bytes, 1, 2, file) != 2) {
		return 0;
	}
	*ch = bytes[0] | (bytes[1] << 8);
	return 1;
}

size_t append_utf8_char(char *buffer, size_t offset, uint32_t codepoint) {
	if (codepoint <= 0x7F) {
		buffer[offset++] = codepoint;
	} else if (codepoint <= 0x7FF) {
		buffer[offset++] = 0xC0 | (codepoint >> 6);
		buffer[offset++] = 0x80 | (codepoint & 0x3F);
	} else if (codepoint <= 0xFFFF) {
		buffer[offset++] = 0xE0 | (codepoint >> 12);
		buffer[offset++] = 0x80 | ((codepoint >> 6) & 0x3F);
		buffer[offset++] = 0x80 | (codepoint & 0x3F);
	} else if (codepoint <= 0x10FFFF) {
		buffer[offset++] = 0xF0 | (codepoint >> 18);
		buffer[offset++] = 0x80 | ((codepoint >> 12) & 0x3F);
		buffer[offset++] = 0x80 | ((codepoint >> 6) & 0x3F);
		buffer[offset++] = 0x80 | (codepoint & 0x3F);
	}
	return offset;
}

char *read_error_log(char *utf8_buffer) {
	FILE *in = fopen("errors.log", "rb");
	if (!in) {
		fprintf(stderr, "Err - unable to open errors.log file for metaeditor.exe\n");
		return NULL;
	}

	size_t offset = 0;
	uint16_t ch;

	while (read_utf16_char_file(in, &ch)) {
		if (ch >= 0xD800 && ch <= 0xDBFF) {  // High surrogate
			uint16_t low;
			if (!read_utf16_char_file(in, &low) || low < 0xDC00 || low > 0xDFFF) {
				printf("Invalid surrogate pair.\n");
				continue;
			}
			uint32_t codepoint = 0x10000 + (((ch - 0xD800) << 10) | (low - 0xDC00));
			offset = append_utf8_char(utf8_buffer, offset, codepoint);
		}
		else if (ch >= 0xDC00 && ch <= 0xDFFF) {  // Isolated low surrogate
			printf("Isolated low surrogate detected.\n");
		}
		else {
			offset = append_utf8_char(utf8_buffer, offset, ch);
		}
	}

	utf8_buffer[offset] = '\0';  // Null-terminate the buffer

	fclose(in);
	return utf8_buffer;
}

char *get_file_line(char *file, int iLine, char final_line[MAX_TOKEN_SIZE]) {
	FILE* filePointer;
	int line_num = 1;

	filePointer = fopen(file, "r");

	if (filePointer == NULL)
		return "";

	while(fgets(final_line, MAX_TOKEN_SIZE, filePointer)) {
		if (line_num++ == iLine) {
			return final_line;
		}
	}

	fclose(filePointer);

	return "";
}


// Assumes 'result' to be an unallocated buffer
char *generate_error_indicator(char *original_text, char **result) {
	char *text = strdup(original_text);
	char *relevant_identifier = "";
	if (strstr(text, ") : error"))
		relevant_identifier = split_get_first_half(text, ") : error");
	else if (strstr(text, ") : information"))
		relevant_identifier = split_get_first_half(text, ") : information");
	else if (strstr(text, ") : warning"))
		relevant_identifier = split_get_first_half(text, ") : warning");
	else {
		free(text);
		return "";
	}

	int char_index = 0;
	int line_index = 0;
	char *file_name = "";

	array *tokens_out = NEW_ARRAY(MAX_ARRAY_SIZE);
	int token_count = split_string(relevant_identifier, ',', tokens_out);
	char *char_string = tokens_out->array[token_count - 1];
	char_index = atoi(char_string);

	char *line_string = strdup(tokens_out->array[token_count - 2]);
	token_count = split_string(line_string, '(', tokens_out);
	char *old_line_string = line_string;
	line_string = strdup(tokens_out->array[token_count - 1]);
	free(old_line_string);
	line_index = atoi(line_string);

	file_name = split_get_first_half(relevant_identifier, "(");
	if (file_name == NULL || line_index == 0 || char_index == 0) {
		free(line_string);
		free(text);
		return "";
	}

	char target_line[MAX_TOKEN_SIZE] = "";
	get_file_line(file_name, line_index, target_line);

	char whitespace[MAX_TOKEN_SIZE] = "";
	int whitespace_index = 0;
	char_index += 1 + strlen(line_string);
	for (; whitespace_index < char_index; whitespace_index++) {
		whitespace[whitespace_index] = ' ';
	}
	for (; whitespace_index < char_index + 4; whitespace_index++) {
		whitespace[whitespace_index] = '^';
	}

	whitespace[++whitespace_index] = '\n';
	whitespace[++whitespace_index] = '\0';
	int result_size = strlen("\n: \n") + strlen(line_string) + strlen(trim(target_line)) + strlen(whitespace);

	if (*result == NULL)
		free(*result);

	*result = malloc(result_size + sizeof(char));
	snprintf(*result, result_size, "\n%s: %s\n%s", line_string, trim(target_line), whitespace);

	free_array(&tokens_out);
	free(text);
	free(line_string);
	return *result;
}

char *colour_compiler_text(char *text, PRINT_COLOR colour, char final_string[MAX_TOKEN_SIZE]) {
	char *error_text = NULL;
	switch (colour) {
		case NONE:
			if (colourful == true) {
				if (strstr(text, ": information") != NULL) {
					snprintf(final_string, MAX_TOKEN_SIZE, "\x1B[36m%s%s\x1B[0m", text, generate_error_indicator(text, &error_text));
					goto exit;
				}
				else if (strstr(text, ": error") != NULL) {
					snprintf(final_string, MAX_TOKEN_SIZE, "\x1B[31m%s%s\x1B[0m", text, generate_error_indicator(text, &error_text));
					goto exit;
				}
				else if (strstr(text, ": warning") != NULL) {
					snprintf(final_string, MAX_TOKEN_SIZE, "\x1B[33m%s%s\x1B[0m", text, generate_error_indicator(text, &error_text));
					goto exit;
				}
				else if (strstr(text, "Result:") != NULL) {
					snprintf(final_string, MAX_TOKEN_SIZE, "\x1B[32m%s%s\x1B[0m", text, generate_error_indicator(text, &error_text));
					goto exit;
				}
			}
			else {
				snprintf(final_string, MAX_TOKEN_SIZE, "%s%s", text, generate_error_indicator(text, &error_text));
				goto exit;
			}
			break;
		case RED:
			snprintf(final_string, MAX_TOKEN_SIZE, "\x1B[31m%s\x1B[0m", text);
			goto exit;
		case ORANGE:
			snprintf(final_string, MAX_TOKEN_SIZE, "\x1B[33m%s\x1B[0m", text);
			goto exit;
		case CYAN:
			snprintf(final_string, MAX_TOKEN_SIZE, "\x1B[36m%s\x1B[0m", text);
			goto exit;
		case GREEN:
			snprintf(final_string, MAX_TOKEN_SIZE, "\x1B[32m%s\x1B[0m", text);
			goto exit;
	}

	strcpy(final_string, text);

	exit:
		if (error_text != NULL)
			free(error_text);
		return final_string;
}


int compile_file(char target_file[MAX_TOKEN_SIZE]) {
	char *popen_command;
	FILE *proc;
	char final_response[MAX_TOKEN_SIZE * 10] = "";

	if (use_wine) {
		char new_file[MAX_TOKEN_SIZE];
		int conversion_success = convert_wine_path(target_file, new_file);
		if (conversion_success == 400)
			strcpy(new_file, target_file);
		int command_size = strlen("wine \"\" /compile:\"\" /log:errors.log 2>&1") + strlen(meta_editor) + strlen(new_file) + 1;
		popen_command = malloc(command_size * sizeof(char));
		snprintf(popen_command, command_size, "wine \"%s\" /compile:\"%s\" /log:errors.log 2>&1", meta_editor, new_file);
		proc = popen(popen_command, "r");
	}
	else {
		int command_size = strlen("\"\"\" /compile:\"\" /log:errors.log 2>&1\"") + strlen(meta_editor) + strlen(target_file) + 1;
		popen_command = malloc(command_size * sizeof(char));
		snprintf(popen_command, command_size, "\"\"%s\" /compile:\"%s\" /log:errors.log 2>&1\"", meta_editor, target_file);
		proc = popen(popen_command, "r");
	}
	printf("Compiling file '%s' with '%s'\n", target_file, popen_command);
	free(popen_command);

	char ch;
	while((ch = fgetc(proc)) != EOF) {
		if (strlen(final_response) < (MAX_PATH_LEN - 1)) {
			final_response[strlen(final_response)] = ch;
		}
	}
	final_response[strlen(final_response) + 1] = '\0';
	pclose(proc);

	if (proc && !suppress_launch_errors) {
		char coloured_text[MAX_TOKEN_SIZE];
		printf("%s\n", colour_compiler_text("\nmetaeditor.exe errors:", RED, coloured_text));
		printf("%s\n", final_response);
	}

	printf("COMPILER OUT:\n");

	char file_data[MAX_TOKEN_SIZE * MAX_ARRAY_SIZE];
	read_error_log(file_data);
	array *lines = NEW_ARRAY(MAX_ARRAY_SIZE);
	int lines_len = split_string(file_data, '\n', lines);
	for (int i = 1; i < lines_len; i++) {
		if (!string_isspace(lines->array[i])) {
			char final_line[MAX_TOKEN_SIZE];
			printf("%s\n", colour_compiler_text(trim(lines->array[i]), NONE, final_line));
		}
	}

	free_array(&lines);
	return 200;
}

void compile_files(array *target_files) {
	int success;
	int i = 0;
	char *file;
	ARRAY_FOREACH(file, target_files, i) {
		success = compile_file(file);
		if (success == 400)
			return;
	}
}
