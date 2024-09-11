#include "signal_handlers.h"

volatile sig_atomic_t foreground_pid = 0;

void setup_signal_handlers()
{
    signal(SIGINT, sigint_handler);
    signal(SIGTSTP, sigtstp_handler);
}

void sigint_handler(int signum)
{
    // If there is a foreground process running, send SIGINT to it
    if (foreground_pid != 0)
    {
        kill(foreground_pid, SIGINT);
        foreground_pid = 0;
        printf("\n");
    }
}

void sigtstp_handler(int signum)
{
    if (foreground_pid != 0)
    {
        kill(foreground_pid, SIGTSTP);
        printf("\nProcess %d stopped\n", foreground_pid);

        // Update status in allProcesses list
        processStruct *temp = allProcesses;
        while (temp != NULL) {
            if (temp->pid == foreground_pid) {
                strcpy(temp->status, "Stopped");
                break;
            }
            temp = temp->next;
        }

        // Add the process to bgProcesses list if it's not already there
        temp = bgProcesses;
        bool found = false;
        while (temp != NULL) {
            if (temp->pid == foreground_pid) {
                found = true;
                break;
            }
            temp = temp->next;
        }
        if (!found) {
            processStruct *stoppedProcess = createProcessStruct(foreground_pid, "Unknown");
            strcpy(stoppedProcess->status, "Stopped");
            addProcess(stoppedProcess, &bgProcesses);
        }

        // Ensure shell regains control of the terminal
        tcsetpgrp(STDIN_FILENO, getpgrp());

        foreground_pid = 0;
    }
}

void handle_eof()
{
    printf(BOLD BLUE "\nLogging out of Arihant.sh...\n" RESET);

    freeProcesses(bgProcesses);
    freeProcesses(allProcesses);

    exit(0);
}