#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "colors.h"

// Log the last 15 commands
void add_to_log(char *input_str, const char *home_dir);

// Open the log file and print the contents
void print_log(const char *home_dir);

// Clear the log file
void purge(const char *home_dir);

#endif