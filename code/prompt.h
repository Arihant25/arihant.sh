#ifndef PROMPT_H
#define PROMPT_H

#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <string.h>
#include "colors.h"

// Print the prompt
void prompt(const char *home_dir, char *last_command);

#endif