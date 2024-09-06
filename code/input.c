#include "input.h"

int input(char *input_str)
{
    setup_signal_handlers();

    if (fgets(input_str, 4096, stdin) == NULL)
        return EOF; // Return EOF if fgets fails (this includes EOF condition)

    // Remove the trailing newline character
    input_str[strcspn(input_str, "\n")] = 0;

    return 0; // Return 0 for success
}