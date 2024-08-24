#include "log.h"

void add_to_log(char *input_str, const char *home_dir)
{
    char log_path[4096];
    snprintf(log_path, sizeof(log_path), "%s/.log", home_dir);

    // Open the log file
    FILE *log_file = fopen(log_path, "a+");
    if (log_file == NULL)
    {
        printf(RED "Error: Could not open or create log file\n" RESET);
        return;
    }

    // Count the number of lines
    char line[4096];
    int num_lines = 0;
    while (fgets(line, sizeof(line), log_file) != NULL)
        num_lines++;

    // If the number of lines is more than 15, remove the first line
    if (num_lines >= 15)
    {
        char temp_path[4096];
        snprintf(temp_path, sizeof(temp_path), "%s/.temp", home_dir);
        FILE *temp_file = fopen(temp_path, "w");
        if (temp_file == NULL)
        {
            printf(RED "Error: Could not create temporary file\n" RESET);
            fclose(log_file);
            return;
        }

        // Seek to the beginning of the log file
        if (fseek(log_file, 0, SEEK_SET) != 0)
        {
            printf(RED "Error: Could not seek to the beginning of the file\n" RESET);
            fclose(log_file);
            fclose(temp_file);
            return;
        }

        // Copy all lines except the first to the temporary file
        int i = 0;
        while (fgets(line, sizeof(line), log_file) != NULL)
        {
            if (i != 0)
                fputs(line, temp_file);
            i++;
        }

        // Close both files
        fclose(log_file);
        fclose(temp_file);

        // Replace the old log file with the new one
        if (remove(log_path) != 0 || rename(temp_path, log_path) != 0)
        {
            printf(RED "Error: Could not update log file\n" RESET);
            return;
        }

        // Reopen the log file for appending
        log_file = fopen(log_path, "a");
        if (log_file == NULL)
        {
            printf(RED "Error: Could not reopen log file\n" RESET);
            return;
        }
    }

    // Add the new command to the log if it's not the same as the last command
    line[strlen(line) - 1] = '\0';
    if (num_lines == 0 || strcmp(line, input_str) != 0)
        fprintf(log_file, "%s\n", input_str);

    fclose(log_file);
}

void print_log(const char *home_dir)
{
    char log_path[4096];
    snprintf(log_path, sizeof(log_path), "%s/.log", home_dir);

    FILE *log_file = fopen(log_path, "r");
    if (log_file == NULL)
    {
        printf(RED "Error: Log file not found or not created yet. Try running some commands first!\n" RESET);
        return;
    }

    char line[4096];
    while (fgets(line, sizeof(line), log_file))
        printf("%s", line);
    fclose(log_file);
}

void purge(const char *home_dir)
{
    char log_path[4096];
    snprintf(log_path, sizeof(log_path), "%s/.log", home_dir);

    if (fopen(log_path, "w") == NULL)
        printf(RED "Error: Could not clear log file\n" RESET);

    printf(GREEN "Log file cleared\n" RESET);
}