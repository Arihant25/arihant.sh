#include "scripter.h"

void comment_cleaner()
{
    FILE *dirtyscript = fopen(".myshrc", "r");
    if (dirtyscript == NULL)
    {
        printf(RED "Error: Cannot open file .myshrc\n" RESET);
        return;
    }

    FILE *cleanscript = fopen("temp.myshrc", "w");
    if (cleanscript == NULL)
    {
        printf(RED "Error: Cannot open file temp.myshrc\n" RESET);
        fclose(dirtyscript);
        return;
    }

    char line[4096];

    while (fgets(line, sizeof(line), dirtyscript))
    {
        char *comment = strchr(line, '#');
        if (comment)
            *comment = '\0';
        fprintf(cleanscript, "%s\n", line);
    }

    fclose(dirtyscript);
    fclose(cleanscript);
}

void scripter(char *aliases[])
{
    // Clean the script file
    comment_cleaner();

    // Open the uncommented script file
    FILE *script = fopen("temp.myshrc", "r");
    if (script == NULL)
    {
        printf(RED "Error: Cannot open file temp.myshrc\n" RESET);
        return;
    }

    // Read the script file line by line
    char line[4096];
    int alias_index = 0;

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
                char *name = strtok_r(alias_def, " ", &save_ptr); // Get the alias name
                char *value = equals_sign + 2;                    // Get the alias value
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
    }
    // Null-terminate the array
    aliases[alias_index] = NULL;

    // Close the script file
    fclose(script);
}

char *get_alias(char *aliases[], char *alias)
{
    for (int i = 0; aliases[i] != NULL; i += 2)
        if (strcmp(aliases[i], alias) == 0)
            return aliases[i + 1];
    return NULL;
}