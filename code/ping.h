#ifndef PING_H
#define PING_H

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "colors.h"

// Send a signal to a process
void ping(char **args, int num_args);

#endif