#ifndef SIGNAL_HANDLERS_H
#define SIGNAL_HANDLERS_H

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#include "processes.h"

extern volatile sig_atomic_t foreground_pid;

// Set up signal handlers for SIGINT and SIGTSTP
void setup_signal_handlers();

// Signal handler for SIGINT (Ctrl-C)
void sigint_handler(int signum);

// Signal handler for SIGTSTP (Ctrl-Z)
void sigtstp_handler(int signum);

// Signal handler for EOF (Ctrl-D)
void handle_eof();

#endif