#include "prompt.h"
#include "input.h"
#include "hop.h"
#include "commander.h"

int main()
{
    // Get the home directory
    char cwd[4096];
    getcwd(cwd, sizeof(cwd));
    const char *home_dir = strdup(cwd);
    char *prev_dir = strdup(cwd);

    while (1)
    {
        // Print the prompt
        prompt(home_dir);

        // Get the input string
        char input_str[4096];
        input(input_str);

        // Execute the command
        commander(input_str, home_dir, &prev_dir);
    }

    return 0;
}