#ifndef COMMANDER_H
#define COMMANDER_H

#include "input.h"
#include "hop.h"
#include "colors.h"
#include "reveal.h"
#include "log.h"
#include "proclore.h"
#include "seeker.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Execute the command
void commander(char *input_str, const char *home_dir, char **prev_dir);
#endif
