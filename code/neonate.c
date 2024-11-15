#include "neonate.h"

void neonate(char *args[], int num_args)
{
    if (num_args != 2)
    {
        printf(RED "Error: Invalid number of arguments for neonate\n" RESET);
        return;
    }

    if (strcmp(args[0], "-n") == 0)
    {
        // Get the time interval
        int time_arg = atoi(args[1]);
        if (time_arg <= 0)
        {
            printf(RED "Error: Invalid time interval for neonate\n" RESET);
            return;
        }

        while (true)
        {
            // Check if the x key has been pressed
            if (kbhit())
            {
                char c = getchar();
                if (c == 'x')
                {
                    printf("\n");
                    return;
                }
            }

            // Get the PID of the newest process
            pid_t newest_pid = get_newest_pid();
            if (newest_pid > 0)
                printf("%d\n", newest_pid);
            else
                printf(RED "Error: Could not get the newest PID\n" RESET);
            sleep(time_arg);
        }
    }
    else
    {
        printf(RED "Error: Invalid argument for neonate\n" RESET);
        return;
    }
}

bool kbhit()
{
    struct termios oldt, newt; // Save the old terminal settings and apply new settings
    int ch;                    // Store the character read
    int oldf;                  // Store the file descriptor settings

    // Save the current terminal settings
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // Disable canonical mode and echo
    newt.c_lflag &= ~(ICANON | ECHO);

    // Apply the new terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Set the file descriptor to non-blocking mode
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    // Check if there is any input available
    ch = getchar();

    // Restore the original terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    // Check if a character was read successfully
    if (ch != EOF)
    {
        // Put the character back into the input stream
        ungetc(ch, stdin);
        return true;
    }

    return false;
}

pid_t get_newest_pid()
{
    struct dirent *entry;
    pid_t newest_pid = 0;
    time_t newest_time = 0;

    // Open the /proc directory
    DIR *proc_dir = opendir("/proc");
    if (proc_dir == NULL)
    {
        perror(RED "Error opening /proc directory" RESET);
        return -1;
    }

    // Read each entry in the /proc directory
    while ((entry = readdir(proc_dir)) != NULL)
    {
        {
            char *endptr;                                  // Pointer to the end of the string
            long pid = strtol(entry->d_name, &endptr, 10); // Convert the string to a number
            if (*endptr == '\0')                           // Check if the entire string was converted
            {
                // Construct the path to the stat file
                char stat_path[4096];
                snprintf(stat_path, sizeof(stat_path), "/proc/%ld/stat", pid);

                // Open the stat file
                FILE *stat_file = fopen(stat_path, "r");
                if (stat_file)
                {
                    // Read the process name from the stat file
                    struct stat st;
                    if (fstat(fileno(stat_file), &st) == 0)
                    {
                        // Check if the process was started after the current newest process
                        if (st.st_ctime > newest_time)
                        {
                            // Update the newest process information
                            newest_time = st.st_ctime;
                            newest_pid = pid;
                        }
                    }
                    fclose(stat_file);
                }
            }
        }
    }

    closedir(proc_dir);
    return newest_pid;
}
