#ifndef NEONATE_H
#define NEONATE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "colors.h"

// Print the PID of the most recently executed command every time_arg seconds
void neonate(char *args[], int num_args);

// Check if the x key has been pressed
bool kbhit();

// Get the PID of the newest process
pid_t get_newest_pid();

#endif