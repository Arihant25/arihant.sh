#ifndef COMMANDER_H
#define COMMANDER_H

#define MAX_PIPES 4096

#include "input.h"
#include "hop.h"
#include "colors.h"
#include "reveal.h"
#include "log.h"
#include "proclore.h"
#include "seeker.h"
#include "iman.h"
#include "neonate.h"
#include "processes.h"
#include "activities.h"
#include "ping.h"
#include "signal_handlers.h"
#include "fgbg.h"
#include "scripter.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <fcntl.h>
#include <termios.h>

// Save the original file descriptors
void save_original_fds();

// Restore the original file descriptors
void restore_stdin_stdout();

// Check if the command contains redirection
bool check_redirection(char **args, int num_args, char **input_file,
                       char **output_file, int *append_output);

// Handle redirection
void handle_redirection(char *input_file, char *output_file,
                        int append_output, int *num_args);

// Execute a single command
void execute_command(char **args, char *input_file, char *output_file, int append_output,
                     const char *home_dir, char **prev_dir, char **last_command, char *aliases[4096]);

// Execute piped commands
void execute_piped_commands(char *command, const char *home_dir, char **prev_dir,
                            char **last_command, char *aliases[4096]);

// Execute the command
void commander(char *input_str, const char *home_dir, char **prev_dir,
               char **last_command, char *aliases[4096]);

// Execute one of the custom commands
bool execute_custom_command(char **args, int num_args, const char *home_dir,
                            char **prev_dir, char **last_command, char *aliases[4096]);

#endif
