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
#include <time.h>

// Struct to store background process information
typedef struct BackgroundProcess
{
    pid_t pid;
    char name[4096];
    struct BackgroundProcess *next;
} BackgroundProcess;

extern BackgroundProcess *bgProcesses; // Head of the linked list of background processes

// Create a new background process
BackgroundProcess *createBgProcess(pid_t pid, char *name);

// Add a background process to the linked list
void addBgProcess(pid_t pid, char *name, BackgroundProcess *head, BackgroundProcess *newBgProcess);

// Check if a process is running in the background
bool checkBgProcesses();

// Execute the command
void commander(char *input_str, const char *home_dir, char **prev_dir, char **last_command);
#endif
