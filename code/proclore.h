#ifndef PROCLORE_H
#define PROCLORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <termios.h>
#include "colors.h"

// Get process information
void proclore(char **args, int num_args);

#endif