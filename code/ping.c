#include "ping.h"

void ping(char **args, int num_args)
{
    if (num_args != 2)
    {
        printf(RED "Error: Invalid number of arguments for ping (required 2, got %d)\n" RESET, num_args);
        return;
    }

    int pid = atoi(args[0]);
    int signal = atoi(args[1]);

    // Check if the process exists
    if (kill(pid, 0) == -1)
    {
        printf(RED "Error: No such process found\n" RESET);
        return;
    }

    // Send the signal
    signal = abs(signal) % 32; // Ensure the signal is within the range 0-31
    if (kill(pid, signal) == -1)
        printf(RED "Error: Could not send signal %d to process with PID %d\n" RESET, signal, pid);
    else
        printf(GREEN "Signal %d sent to process with PID %d\n" RESET, signal, pid);
}