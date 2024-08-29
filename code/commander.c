#include "commander.h"

BackgroundProcess *bgProcesses = NULL;

BackgroundProcess *createBgProcess(pid_t pid, char *name)
{
    BackgroundProcess *bgProcess = (BackgroundProcess *)malloc(sizeof(BackgroundProcess));
    if (bgProcess == NULL)
    {
        printf(RED "Error: Memory allocation failed for background process\n" RESET);
        return NULL;
    }

    bgProcess->pid = pid;
    strcpy(bgProcess->name, name);
    bgProcess->next = NULL;

    return bgProcess;
}

void addBgProcess(pid_t pid, char *name, BackgroundProcess *head, BackgroundProcess *newBgProcess)
{
    if (head == NULL)
    {
        bgProcesses = newBgProcess;
        return;
    }

    BackgroundProcess *temp = head;
    while (temp->next != NULL)
        temp = temp->next;

    temp->next = newBgProcess;
}

bool checkBgProcesses()
{
    BackgroundProcess *temp = bgProcesses;
    BackgroundProcess *prev = NULL;

    while (temp != NULL)
    {
        int status;
        pid_t result = waitpid(temp->pid, &status, WNOHANG);
        if (result == -1)
        {
            printf(RED "Error: waitpid failed\n" RESET);
            return false;
        }

        if (result > 0)
        {
            if (status == 0)
                printf("%s exited normally (%d)\n", temp->name, temp->pid);
            else
                printf("%s exited abnormally (%d)\n", temp->name, temp->pid);

            // Remove the process from the linked list
            if (prev == NULL) // The process is the head of the linked list
            {
                bgProcesses = temp->next;
                free(temp);
                temp = bgProcesses;
            }
            else
            {
                prev->next = temp->next;
                free(temp);
                temp = prev->next;
            }
        }
        else
        {
            prev = temp;
            temp = temp->next;
        }
    }
}

void commander(char *input_str, const char *home_dir, char **prev_dir, char **last_command)
{
    checkBgProcesses();

    // Set the start time
    time_t start = time(NULL);

    // If the input string contains 'log', don't add it to the log
    if (strstr(input_str, "log") == NULL)
        add_to_log(input_str, home_dir);

    // Tokenise the input string and separate the commands
    char *commands[4096]; // Array to store the commands

    char *saveptr;
    char *command = strtok_r(input_str, ";", &saveptr);

    // Store the commands in the array
    int num_commands = 0;
    while (command != NULL)
    {
        commands[num_commands++] = command;
        command = strtok_r(NULL, ";", &saveptr);
    }

    // Add NULL at the end of the array
    commands[num_commands] = NULL;

    /*
    If the input was command_a arg_1 ; command_b arg_1 arg_2
    commands = [command_a arg_1, command_b arg_1 arg_2]
    */

    for (int j = 0; j < num_commands; j++) // For each command
    {
        // Tokenise the command and get the arguments
        char *args[4096];
        char *arg = strtok_r(commands[j], " ", &saveptr);
        int num_args = 0;
        while (arg != NULL)
        {
            args[num_args++] = arg;
            arg = strtok_r(NULL, " ", &saveptr);
        }

        // Check if the last argument is &
        int background = false;
        if (strcmp(args[num_args - 1], "&") == 0)
        {
            background = true;
            args[num_args - 1] = NULL; // Remove the & from the arguments
            num_args--;
        }

        // Execute the command

        // Change directories
        if (strcmp(args[0], "hop") == 0)
            hop(args + 1, num_args - 1, home_dir, prev_dir);

        // List directories
        else if (strcmp(args[0], "reveal") == 0)
            reveal(args + 1, num_args - 1, home_dir, prev_dir);

        // Print the history of commands
        else if (strcmp(args[0], "log") == 0)
        {
            if (num_args == 1)
            {
                print_log(home_dir);
                return;
            }

            else if (strcmp(args[1], "execute") == 0)
            {

                if (num_args != 3)
                {
                    printf(RED "Error: Invalid number of arguments for log execute (required 1, got %d)\n" RESET, num_args - 2);
                    return;
                }

                int index = atoi(args[2]);
                char log_path[4096];
                snprintf(log_path, sizeof(log_path), "%s/.log", home_dir);
                FILE *log_file = fopen(log_path, "r");
                if (log_file == NULL)
                {
                    printf(RED "Error: Log file not found\n" RESET);
                    return;
                }

                // Get the command at the given index from the end
                char line[4096];
                int num_lines = 0;
                while (fgets(line, sizeof(line), log_file) != NULL)
                    num_lines++;

                if (index < 1 || index > num_lines)
                {
                    if (num_lines == 0)
                        printf(RED "Error: Log file is empty\n" RESET);
                    else if (num_lines == 1)
                        printf(RED "Error: Invalid index for log execute (required 1, got %d)\n" RESET, index);
                    else
                        printf(RED "Error: Invalid index for log execute (required 1-%d, got %d)\n" RESET, num_lines, index);
                    return;
                }

                // Seek to the beginning of the file
                if (fseek(log_file, 0, SEEK_SET))
                {
                    printf(RED "Error: Could not seek to the beginning of the file\n" RESET);
                    return;
                }

                // Get the command at the given index
                int i = 0;
                while (fgets(line, sizeof(line), log_file))
                {
                    if (i == num_lines - index)
                        break;
                    i++;
                }

                fclose(log_file);

                line[strlen(line) - 1] = '\0'; // Remove the newline character

                // Execute the command
                char *command_copy = strdup(line);
                if (command_copy == NULL)
                {
                    printf(RED "Error: Memory allocation failed\n" RESET);
                    return;
                }
                commander(command_copy, home_dir, prev_dir, last_command);
                free(command_copy);
            }

            else if (strcmp(args[1], "purge") == 0)
            {
                if (num_args != 2)
                {
                    printf(RED "Error: Invalid number of arguments for log purge (required 0, got %d)\n" RESET, num_args - 2);
                    return;
                }

                purge(home_dir);
            }

            else
                printf(RED "Error: Invalid log command\n" RESET);
        }

        // Get process information
        else if (strcmp(args[0], "proclore") == 0)
            proclore(args + 1, num_args - 1);

        // Search for files or directories
        else if (strcmp(args[0], "seek") == 0)
            seeker(args + 1, num_args - 1, home_dir);

        // TODO: Show all processes
        // else if (strcmp(args[0], "activities") == 0)
        // activities();

        // TODO: Send signals to a process
        // else if (strcmp(args[0], "ping") == 0)
        // ping(args + 1, num_args - 1);

        // TODO: Bring a process to the foreground
        // else if (strcmp(args[0], "fg") == 0)
        // fg(args + 1, num_args - 1);

        // TODO: Send a process to the background
        // else if (strcmp(args[0], "bg") == 0)
        // bg(args + 1, num_args - 1);

        // TODO: Print the PID of the most recently executed command
        // else if (strcmp(args[0], "neonate") == 0)
        // neonate(args + 1, num_args - 1);

        // TODO: Show man page
        // else if (strcmp(args[0], "iMan") == 0)
        // iMan(args + 1, num_args - 1);

        // Exit the shell
        else if (strcmp(args[0], "exit") == 0)
            _exit(0);

        // Execute other commands
        else
        {
            pid_t pid = fork();
            if (pid == 0) // Child process
            {
                if (execvp(args[0], args) == -1)
                {
                    printf(RED "ERROR : '%s' is not a valid command\n" RESET, args[0]);
                    fflush(stdout);
                    _exit(1); // indicate error in child process
                }
                _exit(0);
            }

            else if (pid < 0)
                printf(RED "ERROR : Fork failed\n" RESET);

            else // Parent process
                if (background)
                {
                    int status;
                    waitpid(pid, &status, WNOHANG);

                    if (status && status == 0)
                    {
                        printf("%d\n", pid);
                        addBgProcess(pid, args[0], bgProcesses, createBgProcess(pid, args[0]));
                    }
                    // If the child process exited with a non-zero status, we don't print the PID or add it to bgProcesses
                }
                else
                    waitpid(pid, NULL, 0);
        }

        // Set the end time
        time_t end = time(NULL);

        // Calculate the time taken
        double time_taken = difftime(end, start);

        // Store the command and time taken if foreground process
        if (!background)
            snprintf(*last_command, 4096, "%s : %.2fs", commands[j], time_taken);
        else
            (*last_command)[0] = '\0';

        // Clear the args array so that it can be reused
        for (int k = 0; k < num_args; k++)
            args[k] = NULL;
    }
}
