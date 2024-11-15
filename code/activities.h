#ifndef ACTIVITIES_H
#define ACTIVITIES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/types.h>

#include "colors.h"
#include "processes.h"

// Print the list of all processes spawned by the shell in lexicographic order
void activities(struct processStruct *allProcesses);

#endif