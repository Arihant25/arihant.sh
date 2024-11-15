#include "prompt.h"

void prompt(const char *home_dir, char *last_command)
{
    // Get the hostname
    char hostname[4096];
    gethostname(hostname, 4096); // Get the hostname with buffer size 4096

    // Get the username
    struct utsname u; // Struct to store the system name
    uname(&u);        // Get the system name into the struct

    struct passwd *p;       // Struct to store the username
    p = getpwuid(getuid()); // Get the username
    if (p == NULL)
    {
        perror("Could not get username");
        return;
    }

    // Get the current working directory
    char cwd[4096];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("Could not get current working directory");
        return;
    };

    char dir_display[4096] = "~"; // Displayed directory

    // If the current directory is a subdirectory of the home directory
    if (strncmp(cwd, home_dir, strlen(home_dir)) == 0)
    {
        if (strlen(cwd) > strlen(home_dir))                                            // If the current directory is not the home directory
            snprintf(dir_display, sizeof(dir_display), "~%s", cwd + strlen(home_dir)); // Display the directory relative to the home directory
    }
    else // Display the full directory
    {
        strncpy(dir_display, cwd, sizeof(dir_display) - 1);
        dir_display[sizeof(dir_display) - 1] = '\0';
    }

    // Get the last command name and time
    char command_name[4096] = "";
    char command_time[4096] = "";
    if (last_command[0] != '\0') // If there is a last command
    {
        char *colon = strchr(last_command, ':'); // Find the colon in the last command
        if (colon != NULL)
        {
            size_t name_len = colon - last_command; // Get the length of the command name

            strncpy(command_name, last_command, name_len); // Copy the command name
            command_name[name_len] = '\0';

            strncpy(command_time, colon + 2, sizeof(command_time) - 1); // Copy the command time
            command_time[sizeof(command_time) - 1] = '\0';
        }

        // Print the prompt
        printf("<"BOLD YELLOW "%s" RESET GREEN "@%s:" CYAN "%s " MAGENTA "%s: " BLUE "%s> " RESET, p->pw_name, u.nodename, dir_display, command_name, command_time);
    }

    else // If there is no last command
        printf("<"BOLD YELLOW "%s" RESET GREEN "@%s:" CYAN "%s" WHITE "> " RESET, p->pw_name, u.nodename, dir_display);

    last_command[0] = '\0'; // Clear the last_command
}