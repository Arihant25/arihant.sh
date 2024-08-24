#include "hop.h"

void hop(char *args[], int num_args, const char home_dir[], char **prev_dir)
{
    // Get current working directory before hop
    char cwd[4096];
    getcwd(cwd, sizeof(cwd));

    // If no arguments are provided, change to home directory
    if (num_args == 0)
    {
        if (chdir(home_dir) != 0)
            printf(RED "ERROR : Could not change to home directory\n" RESET);
        else
            *prev_dir = cwd; // Update previous directory

        printf("%s\n", cwd);
        return;
    }

    for (int i = 0; i < num_args; i++)
    {
        // If the argument is ~, change to home directory
        if (strcmp(args[i], "~") == 0)
            chdir(home_dir);

        // If the argument is -, change to the previous directory
        else if (strcmp(args[i], "-") == 0)
            chdir(*prev_dir);

        // If the argument is a directory path, change to that directory
        else
        {
            // Check if the directory starts with ~
            if (args[i][0] == '~')
            {
            // Create a new path by concatenating home directory and the rest of the path
            char new_path[4096];
            snprintf(new_path, sizeof(new_path), "%s/%s", home_dir, args[i] + 1);

            // Check if the new path exists
            if (access(new_path, F_OK) == 0)
                chdir(new_path);
            else
            {
                printf(RED "ERROR : Directory %s not found\n" RESET, args[i]);
                continue;
            }
            }
            else
            {
            // Check if the directory exists
            if (access(args[i], F_OK) == 0)
                chdir(args[i]);
            else
            {
                printf(RED "ERROR : Directory %s not found\n" RESET, args[i]);
                continue;
            }
            }
        }

        // Update directories
        strcpy(*prev_dir, cwd);
        getcwd(cwd, sizeof(cwd));
        printf("%s\n", cwd);
    }
}