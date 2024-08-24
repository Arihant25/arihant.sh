#ifndef PROMPT_H
#define PROMPT_H

#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <string.h>

// Print the prompt
void prompt(const char *home_dir);

#endif