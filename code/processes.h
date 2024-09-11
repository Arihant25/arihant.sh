#ifndef PROCESSES_H
#define PROCESSES_H

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>

#include "colors.h"

// Struct to store process information
typedef struct processStruct
{
    pid_t pid;
    char name[4096];
    char status[4096];
    struct processStruct *next;
} processStruct;

extern processStruct *bgProcesses;  // Head of the linked list of background processes
extern processStruct *allProcesses; // Head of the linked list of all processes

// Create a new process
processStruct *createProcessStruct(pid_t pid, char *name);

// Add a process to the end of a linked list
void addProcess(processStruct *newProcess, processStruct **head);

// Check if a process is running in the background
void checkBgProcesses(struct processStruct **ProcessList);

// Free the memory allocated for the processes
void freeProcesses(processStruct *head);

#endif