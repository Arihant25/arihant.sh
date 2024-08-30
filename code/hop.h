#ifndef HOP_H
#define HOP_H

#include "colors.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pwd.h>

// Change directories
void hop(char *args[], int num_args, const char *home_dir, char **prev_dir);

#endif