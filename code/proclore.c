#include "proclore.h"

void proclore(char **args, int num_args)
{
    if (num_args > 1)
    {
        printf(RED "Error: Invalid number of arguments for proclore (required 0 or 1, got %d)\n" RESET, num_args - 1);
        return;
    }

    // Display information of the shell
    char path[4096];
    pid_t target_pid;
    if (num_args == 0)
    {
        target_pid = getpid();
        sprintf(path, "/proc/%d/status", target_pid);
    }
    else
    {
        target_pid = atoi(args[0]);
        sprintf(path, "/proc/%s/status", args[0]);
    }

    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        printf(RED "Error: Process %s not found\n" RESET, num_args == 0 ? "shell" : args[0]);
        return;
    }

    char line[4096];
    char pid[4096];
    char state[4096];
    char pgrp[4096];
    char vmSize[4096] = "0";
    char executablePath[4096];

    while (fgets(line, sizeof(line), file))
    {
        char *value = strchr(line, ':');
        if (value != NULL)
        {
            value++;                // Skip the colon
            while (isspace(*value)) // Skip the spaces
                value++;

            // Remove newline character
            char *newline = strchr(value, '\n');
            if (newline != NULL)
                *newline = '\0';

            // PID
            if (strstr(line, "Tgid:") != NULL)
                sprintf(pid, "%s", value);

            // State
            else if (strstr(line, "State:") != NULL)
            {
                if (strlen(value) > 0)
                {
                    // Remove everything after the closing bracket
                    char *opening_bracket = strchr(value, '(');
                    if (opening_bracket != NULL)
                        *(opening_bracket - 1) = '\0';
                    sprintf(state, "%s", value);
                }

                // Check if the process is in foreground or background
                pid_t fg_pgid = tcgetpgrp(STDIN_FILENO);  // Get the process group ID of the foreground process
                pid_t process_pgid = getpgid(target_pid); // Get the process group ID of the target process

                if (strcmp(state, "Z") != 0 && fg_pgid == process_pgid) // If the process is not a zombie and in the foreground
                    strcat(state, "+");
            }

            // Process Group
            else if (strstr(line, "Gid:") != NULL)
            {
                sscanf(value, "%s", pgrp);
                char *space = strchr(pgrp, ' ');
                if (space != NULL)
                    *space = '\0';
            }

            // Virtual Memory
            else if (strstr(line, "VmSize:") != NULL)
                sprintf(vmSize, "%s", value);
        }
    }

    fclose(file);

    // Get the executable path
    char exe_path[4096];
    sprintf(exe_path, "/proc/%d/exe", target_pid);
    int len = readlink(exe_path, executablePath, sizeof(exe_path));
    if (len != -1)
        executablePath[len] = '\0';
    else
        strcpy(executablePath, "Not privileged enough to get the executable path");

    // Display the information
    printf("PID: %s\n", pid);
    printf("Process Status: %s\n", state);
    printf("Process Group: %s\n", pgrp);
    printf("Virtual Memory: %s\n", vmSize);
    printf("Executable Path: %s\n", executablePath);
}