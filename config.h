#ifndef __CONFIG_H
#define __CONFIG_H

#include "cJSON.h"

void process_json_config(cJSON *data_json);
void process_command_flags(int argc, char *argv[]);

#endif
