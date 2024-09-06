#include "fgbg.h"

// FIXME: Kills the shell along with the process
void fg(char **args, int num_args)
{
    if (num_args != 1)
    {
        printf(RED "Error: fg expected 1 argument, got %d." RESET, num_args);
        return;
    }

    int job_id = atoi(args[0]);
    if (job_id <= 0)
    {
        printf(RED "Error: Invalid process ID." RESET);
        return;
    }

    processStruct *job = NULL;
    processStruct *prev = NULL;

    // Find the job in the background processes list
    while (bgProcesses != NULL)
    {
        if (bgProcesses->pid == job_id)
        {
            job = bgProcesses;
            break;
        }
        prev = bgProcesses;
        bgProcesses = bgProcesses->next;
    }

    if (job == NULL)
    {
        printf(RED "Error: No such process found." RESET);
        return;
    }

    // Remove the job from the background processes list
    if (prev == NULL)
        bgProcesses = job->next;
    else
        prev->next = job->next;

    // Move the process to the foreground
    foreground_pid = job->pid;
    printf(GREEN "Bringing process %d (%s) to the foreground\n" RESET, job->pid, job->name);

    // Give terminal control to the process
    tcsetpgrp(STDIN_FILENO, job->pid);

    // Send SIGCONT to the process group
    if (kill(job->pid, SIGCONT) < 0)
    {
        printf(RED "Error: Could not send SIGCONT to the process group\n" RESET);
        return;
    }

    // Wait for the process to finish or stop
    int status;
    waitpid(job->pid, &status, WUNTRACED);

    // Restore terminal control to the shell
    tcsetpgrp(STDIN_FILENO, getpid());

    if (WIFSTOPPED(status))
    {
        printf("Process %d stopped\n", job->pid);
        // Add the job back to the background processes list
        job->next = bgProcesses;
        bgProcesses = job;
    }
    else
    {
        // If the process has terminated, free its memory
        free(job->name);
        free(job);
    }

    foreground_pid = 0;
}

void bg(char **args, int num_args)
{
    if (num_args != 1)
    {
        printf(RED "Error: bg expected 1 argument, got %d.\n" RESET, num_args);
        return;
    }

    int job_id = atoi(args[0]);
    if (job_id <= 0)
    {
        printf(RED "Error: Invalid process ID.\n" RESET);
        return;
    }
    processStruct *job = NULL;

    // Find the job in the background processes list
    while (bgProcesses != NULL)
    {
        if (bgProcesses->pid == job_id)
        {
            job = bgProcesses;
            break;
        }
        bgProcesses = bgProcesses->next;
    }

    if (job == NULL)
    {
        printf(RED "Error: No such process found.\n" RESET);
        return;
    }

    // Send SIGCONT to the process group
    if (kill(job->pid, SIGCONT) < 0)
    {
        printf(RED "Error: Could not send SIGCONT to the process group\n" RESET);
        return;
    }

    printf(GREEN "Continuing process %d (%s) in the background\n" RESET, job->pid, job->name);
}