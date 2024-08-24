#ifndef REVEAL_H
#define REVEAL_H

#include "colors.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <errno.h>

#define MAX_FILES 4096

typedef struct
{
    char *name;
    struct stat st;
} FileInfo;

// List files in a directory
void reveal(char **args, int num_args, const char *home_dir, char **prev_dir);

// Helper function for qsort
int compare(const void *a, const void *b);

// Choose appropriate colour for file type
void printFileColor(struct stat *st);

#endif