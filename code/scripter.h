#ifndef SCRIPTER_H
#define SCRIPTER_H

#include <stdio.h>
#include <string.h>

#include "colors.h"

// Clean the script file by removing comments
void comment_cleaner();

// Read the script file and load the aliases into the array
void scripter(char *aliases[]);

// Check if a given string is an alias, and return its value if it is
char *get_alias(char *aliases[], char *alias);

#endif