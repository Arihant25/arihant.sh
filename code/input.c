#include "input.h"

void input(char *input_str)
{
    // Read the input string
    fgets(input_str, 4096, stdin);

    // Remove the trailing newline character
    input_str[strcspn(input_str, "\n")] = 0;
}
