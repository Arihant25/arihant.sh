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