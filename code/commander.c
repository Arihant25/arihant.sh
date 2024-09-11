#include "commander.h"

processStruct *bgProcesses = NULL;
processStruct *allProcesses = NULL;

// TODO: Implement pipes and redirection for custom commands
// TODO: Implement backgrounding for pipes and redirects (& should work only for the command immediately preceeding it, not for the entire pipe)
// FIXME: Ctrl + Z should actually stop the process

int check_redirection(char **args, int num_args, char **input_file, char **output_file, int *append_output)
{
    *input_file = NULL;  // Input file for redirection
    *output_file = NULL; // Output file for redirection
    *append_output = 0;  // Flag to check if output should be appended
    for (int i = 0; i < num_args - 1; i++)
    {
        if (strcmp(args[i], "<") == 0)
        {
            *input_file = args[i + 1];
            args[i] = NULL;
        }
        else if (strcmp(args[i], ">") == 0)
        {
            *output_file = args[i + 1];
            args[i] = NULL;
        }
        else if (strcmp(args[i], ">>") == 0)
        {
            *output_file = args[i + 1];
            *append_output = 1;
            args[i] = NULL;
        }
    }
    return (*input_file != NULL || *output_file != NULL); // Return true if redirection is used
}

void handle_redirection(char *input_file, char *output_file, int append_output)
{
    if (input_file) // Handle input redirection (<)
    {
        int fd = open(input_file, O_RDONLY);
        if (fd == -1)
        {
            printf(RED "Error: Couldn't open input file\n" RESET);
            exit(1);
        }
        dup2(fd, STDIN_FILENO); // Redirect the input to the file
        close(fd);
    }
    if (output_file) // Handle output redirection (>) or append output (>>)
    {
        int flags = O_WRONLY | O_CREAT;
        if (append_output)
            flags |= O_APPEND;
        else
            flags |= O_TRUNC;

        int fd = open(output_file, flags, 0644);
        if (fd == -1)
        {
            printf(RED "Error: Couldn't open output file\n" RESET);
            exit(1);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
}

void execute_command(char **args, char *input_file, char *output_file, int append_output)
{
    pid_t pid = fork();
    if (pid == 0) // Child process
    {
        handle_redirection(input_file, output_file, append_output);
        execvp(args[0], args);
        printf(RED "ERROR : '%s' is not a valid command\n" RESET, args[0]);
        exit(1);
    }
    else if (pid < 0)
    {
        printf(RED "ERROR : Fork failed\n" RESET);
        exit(1);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
    }
}

void execute_piped_commands(char *command, const char *home_dir, char **prev_dir, char **last_command, char *aliases[4096])
{
    // Trim leading and trailing whitespace
    while (isspace(*command))
        command++;
    char *end = command + strlen(command) - 1;
    while (end > command && isspace(*end))
        end--;
    *(end + 1) = '\0';

    // Check if the command starts or ends with a pipe
    if (command[0] == '|' || command[strlen(command) - 1] == '|')
    {
        printf(RED "Invalid use of pipe\n" RESET);
        return;
    }

    int pipe_count = 0;                 // Number of pipes in the command
    char *pipe_commands[MAX_PIPES + 1]; // Array to store the commands separated by pipes
    char *saveptr;

    // Split the command by pipes
    pipe_commands[pipe_count] = strtok_r(command, "|", &saveptr);
    while (pipe_commands[pipe_count] != NULL && pipe_count < MAX_PIPES)
    {
        // Trim leading and trailing whitespace for each command
        while (isspace(*pipe_commands[pipe_count]))
            pipe_commands[pipe_count]++;
        end = pipe_commands[pipe_count] + strlen(pipe_commands[pipe_count]) - 1;
        while (end > pipe_commands[pipe_count] && isspace(*end))
            end--;
        *(end + 1) = '\0';

        // Check for empty command between pipes
        if (strlen(pipe_commands[pipe_count]) == 0)
        {
            printf(RED "Invalid use of pipe\n" RESET);
            return;
        }

        pipe_count++;
        pipe_commands[pipe_count] = strtok_r(NULL, "|", &saveptr);
    }

    int pipes[MAX_PIPES][2];                 // Array to store the pipe fds
    for (int i = 0; i < pipe_count - 1; i++) // For n commands, there are n - 1 pipes
        if (pipe(pipes[i]) < 0)
        {
            printf(RED "Error creating pipe\n" RESET);
            exit(1);
        }

    for (int i = 0; i < pipe_count; i++)
    {
        char *args[4096];
        int arg_count = 0;
        char *token = strtok(pipe_commands[i], " \t");
        while (token != NULL && arg_count < 4095)
        {
            args[arg_count++] = token;
            token = strtok(NULL, " \t");
        }
        args[arg_count] = NULL;

        char *input_file = NULL, *output_file = NULL;
        int append_output = 0;
        check_redirection(args, arg_count, &input_file, &output_file, &append_output);

        pid_t pid = fork();
        if (pid == 0) // Child process
        {
            // Set up pipes
            if (i > 0)
                dup2(pipes[i - 1][0], STDIN_FILENO);
            if (i < pipe_count - 1)
                dup2(pipes[i][1], STDOUT_FILENO);

            // Close all pipe fds
            for (int j = 0; j < pipe_count - 1; j++)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            handle_redirection(input_file, output_file, append_output);
            execvp(args[0], args);
            printf(RED "ERROR : '%s' is not a valid command\n" RESET, args[0]);
            exit(1);
        }
        else if (pid < 0)
        {
            printf(RED "ERROR : Fork failed\n" RESET);
            exit(1);
        }
    }

    // Close all pipe fds in the parent
    for (int i = 0; i < pipe_count - 1; i++)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all child processes
    for (int i = 0; i < pipe_count; i++)
        wait(NULL);
}

void commander(char *input_str, const char *home_dir, char **prev_dir,
               char **last_command, char *aliases[4096])
{
    checkBgProcesses(&bgProcesses);

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
    commands = [command_a arg_1, command_b arg_1 arg_2, NULL]
    */

    for (int j = 0; j < num_commands; j++)
    {
        // Check if the command contains pipes
        if (strchr(commands[j], '|') != NULL)
            execute_piped_commands(commands[j], home_dir, prev_dir, last_command, aliases);
        else
        {
            char *args[4096];
            int arg_count = 0;
            char *token = strtok(commands[j], " \t");
            while (token != NULL && arg_count < 4095)
            {
                args[arg_count++] = token;
                token = strtok(NULL, " \t");
            }
            args[arg_count] = NULL;

            char *input_file = NULL, *output_file = NULL;
            int append_output = 0;
            if (check_redirection(args, arg_count, &input_file, &output_file, &append_output))
                execute_command(args, input_file, output_file, append_output);
            else
            {
                // Check if the last argument is &
                int background = false;
                int num_args = arg_count;
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
                            printf(RED "Error: Invalid number of arguments for log execute (required 1, got %d)\n" RESET,
                                   num_args - 2);
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
                        commander(command_copy, home_dir, prev_dir, last_command, aliases);
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

                // Show all processes
                else if (strcmp(args[0], "activities") == 0)
                    activities(allProcesses);

                // Send signals to a process
                else if (strcmp(args[0], "ping") == 0)
                    ping(args + 1, num_args - 1);

                // Bring a process to the foreground
                else if (strcmp(args[0], "fg") == 0)
                    fg(args + 1, num_args - 1);

                // Send a process to the background
                else if (strcmp(args[0], "bg") == 0)
                    bg(args + 1, num_args - 1);

                // Print the PID of the most recently executed command
                else if (strcmp(args[0], "neonate") == 0)
                    neonate(args + 1, num_args - 1);

                // Show man page
                else if (strcmp(args[0], "iMan") == 0)
                    iMan(args[1]);

                // Make a directory and change to it
                else if (strcmp(args[0], "mk_hop") == 0)
                {
                    // Make the directory
                    char *new_mkdir_command = (char *)malloc(4096);
                    snprintf(new_mkdir_command, 4096, "mkdir %s", args[1]);
                    commander(new_mkdir_command, home_dir, prev_dir, last_command, aliases);

                    // Change to the directory
                    char *new_hop_command = (char *)malloc(4096);
                    snprintf(new_hop_command, 4096, "hop %s", args[1]);
                    commander(new_hop_command, home_dir, prev_dir, last_command, aliases);

                    // Free the memory allocated for the commands
                    free(new_mkdir_command);
                    free(new_hop_command);
                }

                // Hop into a directory and search for its name
                else if (strcmp(args[0], "hop_seek") == 0)
                {
                    // Change to the directory
                    char *new_hop_command = (char *)malloc(4096);
                    snprintf(new_hop_command, 4096, "hop %s", args[1]);
                    commander(new_hop_command, home_dir, prev_dir, last_command, aliases);

                    // Search for the directory
                    char *new_seek_command = (char *)malloc(4096);
                    snprintf(new_seek_command, 4096, "seek %s", args[1]);
                    commander(new_seek_command, home_dir, prev_dir, last_command, aliases);

                    // Free the memory allocated for the commands
                    free(new_hop_command);
                    free(new_seek_command);
                }

                // Exit the shell
                else if (strcmp(args[0], "exit") == 0)
                {
                    // Free the memory allocated for the processes
                    freeProcesses(bgProcesses);
                    freeProcesses(allProcesses);

                    // Reset the signal handlers to default
                    signal(SIGINT, SIG_DFL);

                    // Remove the uncommented script file
                    remove("temp.myshrc");

                    exit(0);
                }

                // Execute other commands
                else
                {
                    // Check if the command is an alias
                    char *alias_value = get_alias(aliases, args[0]);
                    if (alias_value != NULL)
                    {
                        char *alias_copy = strdup(alias_value);
                        printf(GREEN "Executing alias: %s\n" RESET, alias_copy);
                        commander(alias_copy, home_dir, prev_dir, last_command, aliases);
                        free(alias_copy);
                        continue;
                    }

                    pid_t pid = fork();
                    if (pid == 0) // Child process
                    {
                        // Reset the signal handlers to default for child process
                        signal(SIGINT, SIG_DFL);
                        signal(SIGTSTP, SIG_DFL);

                        setpgid(0, 0); // Set the child as the leader of its own process group

                        if (execvp(args[0], args) == -1)
                        {
                            printf(RED "ERROR : '%s' is not a valid command\n" RESET, args[0]);
                            fflush(stdout);
                            _exit(1);
                        }
                        _exit(0);
                    }
                    else if (pid < 0)
                        printf(RED "ERROR : Fork failed\n" RESET);

                    else // Parent process
                    {
                        setpgid(pid, pid); // Ensure child is in its own process group
                        if (!background)
                        {
                            foreground_pid = pid;

                            // Block SIGTTOU while giving terminal control to child
                            sigset_t mask, oldmask;                  // Signal sets
                            sigemptyset(&mask);                      // Create an empty set
                            sigaddset(&mask, SIGTTOU);               // Add SIGTTOU to the set
                            sigprocmask(SIG_BLOCK, &mask, &oldmask); // Block SIGTTOU

                            if (tcsetpgrp(STDIN_FILENO, pid) == -1) // Give terminal control to child
                                printf(RED "Failed to give terminal control to child\n" RESET);

                            int status; // Status of the child process
                            pid_t result = waitpid(pid, &status, WUNTRACED);
                            if (result == -1)
                                printf(RED "Error: waitpid failed\n" RESET);

                            else if (WIFEXITED(status) || WIFSIGNALED(status))
                            {
                                // Child has terminated, no need to reclaim control
                            }

                            else if (WIFSTOPPED(status)) // Child process was stopped
                            {
                                printf("\nProcess %d stopped\n", pid);
                                addProcess(createProcessStruct(pid, args[0]), &bgProcesses);
                            }

                            // Ensure shell regains control of the terminal
                            if (tcsetpgrp(STDIN_FILENO, getpgrp()) == -1)
                                printf(RED "Failed to regain terminal control\n" RESET);

                            sigprocmask(SIG_SETMASK, &oldmask, NULL); // Unblock SIGTTOU

                            foreground_pid = 0; // Reset the foreground process ID
                        }
                        else // Background process
                        {
                            int status = -1; // Status of the child process
                            waitpid(pid, &status, WNOHANG);

                            // If the child process exited with a zero status
                            if (WIFEXITED(status) == 0)
                            {
                                printf("%d\n", pid);
                                addProcess(createProcessStruct(pid, args[0]), &bgProcesses);
                            }
                            // If the child process exited with a non-zero status, don't do anything
                        }

                        // Add the process to allProcesses
                        addProcess(createProcessStruct(pid, args[0]), &allProcesses);
                    }
                }

                // Set the end time
                time_t end = time(NULL);

                // Calculate the time taken as integer
                int time_taken = difftime(end, start);

                // Store the command and time taken if foreground process
                if (!background && time_taken > 2)
                    snprintf(*last_command, 4096, "%s : %ds", commands[j], time_taken);
                else
                    (*last_command)[0] = '\0';

                // Clear the args array so that it can be reused
                for (int k = 0; k < num_args; k++)
                    args[k] = NULL;
            }
        }

        // Remove zombie processes from the list
        processStruct *temp = allProcesses;
        while (temp != NULL)
        {
            if (waitpid(temp->pid, NULL, WNOHANG) != 0)
                removeProcess(temp->pid, &allProcesses);
            temp = temp->next;
        }
    }
}