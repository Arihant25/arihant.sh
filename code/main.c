#include "prompt.h"
#include "input.h"
#include "commander.h"
#include "scripter.h"

int main()
{
    // Get the home directory
    char cwd[4096];
    getcwd(cwd, sizeof(cwd));
    const char *home_dir = strdup(cwd);
    char *prev_dir = strdup(cwd);
    char **last_command = (char **)malloc(sizeof(char *));
    *last_command = (char *)malloc(4096);
    (*last_command)[0] = '\0';

    // Array of custom aliases and functions
    char *aliases[4096];
    char *functions[4096];

    // Read the script file and load the aliases into the aliases array
    scripter(aliases, functions);

    // print the two arrays
    for (int i = 0; i < 4096; i += 2)
        if (aliases[i] != NULL)
            printf(BLUE "Loaded Alias: %s = %s\n", aliases[i], aliases[i + 1]);

    for (int i = 0; i < 4096; i++)
        if (functions[i] != NULL)
            printf("Loaded Function: %s\n" RESET, functions[i]);

    // TODO: Implement the functionality to execute the functions

    while (1)
    {
        // Print the prompt
        prompt(home_dir, *last_command);

        // Get the input string
        char input_str[4096];
        if (input(input_str) == EOF) {
            handle_eof();
            break;
        }

        // Execute the command
        commander(input_str, home_dir, &prev_dir, last_command);
    }

    return 0;
}