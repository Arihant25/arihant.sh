#include "prompt.h"
#include "input.h"
#include "scripter.h"
#include "commander.h"

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

    // Array of custom aliases
    char *aliases[4096];

    scripter(aliases);

    while (1)
    {
        // Print the prompt
        prompt(home_dir, *last_command);

        // Get the input string
        char input_str[4096];
        if (input(input_str) == EOF)
        {
            handle_eof();
            break;
        }

        // Execute the command
        commander(input_str, home_dir, &prev_dir, last_command, aliases);
    }

    return 0;
}