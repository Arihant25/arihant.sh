#include "seeker.h"

void search_directory(const char *base_path, const char *current_path, const char *target, bool only_files, bool only_dirs, MatchInfo *match, int *match_count)
{
    // Open the directory
    DIR *dir = opendir(current_path);
    if (dir == NULL)
    {
        printf(RED "Error: Could not open directory %s\n" RESET, current_path);
        return;
    }

    // Read the directory
    char path[4096];
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) // For each entry in the directory
    {
        // Skip the current and parent directories
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // Get the full path of the entry
        snprintf(path, sizeof(path), "%s/%s", current_path, entry->d_name);

        // Get the file status
        struct stat st;
        if (stat(path, &st) == -1)
        {
            printf(RED "Error: Could not stat %s\n" RESET, path);
            continue;
        }

        // Check if the entry is a directory or a file
        bool is_dir = S_ISDIR(st.st_mode);
        bool is_file = S_ISREG(st.st_mode);

        // Check if the target is contained in the entry
        if (strstr(entry->d_name, target) != NULL)
        {
            // Check if the entry is a file or a directory
            if ((only_files && !is_file) || (only_dirs && !is_dir))
                continue;

            char relative_path[4096];
            snprintf(relative_path, sizeof(relative_path), "%s", path + strlen(base_path) + 1);

            if (is_dir)
                printf(BLUE "%s/\n" RESET, relative_path);
            else
                printf(GREEN "%s\n" RESET, relative_path);

            (*match_count)++;

            // Store the first match
            if (*match_count == 1)
            {
                strncpy(match->path, path, sizeof(match->path));
                match->is_dir = is_dir;
            }
        }

        // Recursively search the directory
        if (is_dir)
            search_directory(base_path, path, target, only_files, only_dirs, match, match_count);
    }

    closedir(dir);
}

void seeker(char **args, int num_args, const char *home_dir)
{
    if (num_args < 1)
    {
        printf(RED "Error: Invalid number of arguments for seek (at least 1 required, got %d)\n" RESET, num_args);
        return;
    }

    bool only_files = false, only_dirs = false, execute = false;
    char *whatToSearch = NULL, *whereToSearch = NULL;

    // Parse the arguments
    for (int i = 0; i < num_args; i++) // For each argument
    {
        if (args[i][0] == '-') // If it is a flag
        {
            for (int j = 1; args[i][j] != '\0'; j++) // For each character in the flag
            {
                if (args[i][j] == 'd')
                    only_dirs = true;
                else if (args[i][j] == 'f')
                    only_files = true;
                else if (args[i][j] == 'e')
                    execute = true;
                else
                {
                    printf(RED "Error: Invalid option for seeker: -%c\n" RESET, args[i][j]);
                    return;
                }
            }
        }
        else if (!whatToSearch)
            whatToSearch = args[i];
        else if (!whereToSearch)
            whereToSearch = args[i];
        else
        {
            printf(RED "Error: Too many non-flag arguments for seeker\n" RESET);
            return;
        }
    }

    if (!whatToSearch)
    {
        printf(RED "Error: Missing target argument for seeker\n" RESET);
        return;
    }

    whereToSearch = whereToSearch ? whereToSearch : "."; // Default to current directory

    if (only_files && only_dirs)
    {
        printf(RED "Invalid flags!\n" RESET);
        return;
    }

    if (whereToSearch[0] == '~')
    {
        char new_path[4096];
        snprintf(new_path, sizeof(new_path), "%s/%s", home_dir, whereToSearch + 1);
        whereToSearch = strdup(new_path);
    }

    MatchInfo match = {{0}, false};
    int match_count = 0;
    search_directory(whereToSearch, whereToSearch, whatToSearch, only_files, only_dirs, &match, &match_count);

    if (match_count == 0)
        printf("No match found!\n");

    else if (execute && match_count == 1)
        if (match.is_dir)
            if (chdir(match.path) == -1)
                printf(RED "Error: Missing permissions to execute directory\n" RESET);
            else
                printf("Changed directory to: %s\n", match.path);
        else
        {
            FILE *file = fopen(match.path, "r");
            if (file)
            {
                char buffer[4096];
                while (fgets(buffer, sizeof(buffer), file))
                    printf("%s", buffer);
                fclose(file);
                printf("\n");
            }
            else
                printf(RED "Error: Missing permissions to read file\n" RESET);
        }
}