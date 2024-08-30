#ifndef SEEKER_H
#define SEEKER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>

#include "colors.h"

// Structure to store the match information
typedef struct
{
    char path[4096];
    bool is_dir;
} MatchInfo;

// Recursively search the directory for the target
void search_directory(const char *base_path, const char *current_path, const char *target, bool only_files, bool only_dirs, MatchInfo *match, int *match_count);

/*
Arguments

base_path: path from where the search started
current_path: path of the current directory
target:  string to search for
only_files: flag to search only for files
only_dirs: flag to search only for directories
execute: flag to execute the first match
match_count: pointer to the number of matches found
*/

// Parse the arguments and call the search_directory function
void seeker(char **args, int num_args, const char *home_dir);

#endif