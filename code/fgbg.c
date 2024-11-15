#include "fgbg.h"

void fg(char **args, int num_args)
{
    if (num_args != 1)
    {
        printf(RED "Error: fg expected 1 argument, got %d." RESET, num_args);
        return;
    }

    int pid = atoi(args[0]);
    if (pid <= 0)
    {
        printf(RED "Error: Invalid process ID." RESET);
        return;
    }

    // Find the process in /proc
    char proc_path[4096];
    snprintf(proc_path, sizeof(proc_path), "/proc/%d", pid);
    struct stat proc_stat;
    if (stat(proc_path, &proc_stat) < 0)
    {
        printf(RED "Error: No such process found.\n" RESET);
        return;
    }

    // Send SIGCONT
    if (kill(pid, SIGCONT) < 0)
    {
        printf(RED "Error: Could not send SIGCONT to the process group\n" RESET);
        return;
    }

    // Wait for the process to finish
    int status;
    waitpid(pid, &status, WUNTRACED);

    printf(GREEN "Process %d has finished\n" RESET, pid);
}

void bg(char **args, int num_args)
{
    if (num_args != 1)
    {
        printf(RED "Error: bg expected 1 argument, got %d." RESET, num_args);
        return;
    }

    int pid = atoi(args[0]);
    if (pid <= 0)
    {
        printf(RED "Error: Invalid process ID." RESET);
        return;
    }

    // Find the process in /proc
    char proc_path[4096];
    snprintf(proc_path, sizeof(proc_path), "/proc/%d", pid);
    struct stat proc_stat;
    if (stat(proc_path, &proc_stat) < 0)
    {
        printf(RED "Error: No such process found.\n" RESET);
        return;
    }

    // Send SIGCONT
    if (kill(pid, SIGCONT) < 0)
    {
        printf(RED "Error: Could not send SIGCONT to the process group\n" RESET);
        return;
    }

    printf(GREEN "Process %d has been sent to the background\n" RESET, pid);
}