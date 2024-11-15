#ifndef FGBG_H
#define FGBG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <termios.h>
#include <sys/stat.h>

#include "processes.h"
#include "signal_handlers.h"
#include "colors.h"

// Bring a process to the foreground
void fg(char **args, int num_args);

// Send a process to the background
void bg(char **args, int num_args);

#endif