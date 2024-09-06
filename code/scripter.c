#include "scripter.h"

void scripter(char *aliases[], char *functions[])
{
    // Open the script file .myshrc
    FILE *script = fopen(".myshrc", "r");
    if (script == NULL)
    {
        printf(RED "Error: Cannot open file .myshrc\n" RESET);
        return;
    }

    // Read the script file line by line
    char line[4096];
    int alias_index = 0;
    int function_index = 0;

    while (fgets(line, sizeof(line), script))
    {
        // Remove newline character if present
        char *newline = strchr(line, '\n');
        if (newline)
            *newline = '\0';

        // Check if the line is an alias
        if (strncmp(line, "alias", 5) == 0)
        {
            char *alias_def = line + 6; // Skip "alias "
            char *equals_sign = strchr(alias_def, '=');
            if (equals_sign)
            {
                *equals_sign = '\0'; // Split the string at '='
                char *save_ptr;
                char *name = strtok_r(alias_def, " \t", &save_ptr); // Get the alias name
                char *value = equals_sign + 1; // Get the alias value
                while (*value == ' ' || *value == '\t')
                    value++; // Skip leading spaces

                if (alias_index < 4096 - 1)
                {
                    aliases[alias_index++] = strdup(name);
                    aliases[alias_index++] = strdup(value);
                }
                else
                {
                    printf(RED "Error: Too many aliases\n" RESET);
                    break;
                }
            }
        }
        // Check if the line is a function
        else if (strchr(line, '(') && strchr(line, ')'))
        {
            char *function_name = strtok(line, "( \t");
            if (function_name && function_index < 4096)
                functions[function_index++] = strdup(function_name);
        }
    }

    // Null-terminate the arrays
    if (alias_index < 4096)
        aliases[alias_index] = NULL;
    if (function_index < 4096)
        functions[function_index] = NULL;

    // Close the script file
    fclose(script);
}