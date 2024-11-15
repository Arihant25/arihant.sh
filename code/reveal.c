#include "reveal.h"

void reveal(char **args, int num_args, const char *home_dir, char **prev_dir)
{
    int l = false, a = false; // Flags for -l and -a

    // Parse options
    for (int i = 0; i < num_args; i++)
    {
        if (args[i][0] == '-')
            for (int j = 1; args[i][j]; j++)
            {
                if (args[i][j] == 'l')
                    l = true;
                else if (args[i][j] == 'a')
                    a = true;

                if (!l && !a)
                {
                    printf(RED "Error: %s is not a valid option\n" RESET, args[i]);
                    return;
                }
            }
        if (l && a)
            break;
    }

    // Parse path
    char *directory = args[num_args - 1];
    {
        if (args[num_args - 1][0] == '-' && strlen(args[num_args - 1]) > 1 || strcmp(directory, "reveal") == 0)
            directory = ".";
        else
            directory = args[num_args - 1];
    }

    if (strcmp(directory, "-") == 0)
    {
        if (*prev_dir == NULL)
        {
            printf(RED "Error: No previous directory\n" RESET);
            return;
        }
        printf("%s\n", *prev_dir);
        directory = *prev_dir;
    }
    else if (directory[0] == '~') // Replace ~ with home directory
    {

        char *new_dir = malloc(strlen(home_dir) + strlen(directory) + 1); // +1 for null character
        if (new_dir == NULL)
        {
            printf(RED "Error: Memory allocation failed\n" RESET);
            return;
        }
        strcpy(new_dir, home_dir);      // Copy home directory to new_dir
        strcat(new_dir, directory + 1); // Concatenate directory (excluding ~) to new_dir
        directory = new_dir;
    }

    DIR *dir = opendir(directory);
    if (dir == NULL)
    {
        printf(RED "Error: Cannot open directory %s\n" RESET, directory);
        return;
    }

    FileInfo files[MAX_FILES];  // Array to store file information
    int file_count = 0;         // Number of files in the directory
    long long total_blocks = 0; // Total blocks used by the files
    struct dirent *entry;       // Each file in the directory

    while ((entry = readdir(dir)) != NULL && file_count < MAX_FILES)
    {
        if (!a && entry->d_name[0] == '.')
            continue;

        char full_path[4096];
        snprintf(full_path, sizeof(full_path), "%s/%s", directory, entry->d_name);

        if (stat(full_path, &files[file_count].st) == 0)
        {
            files[file_count].name = strdup(entry->d_name);

            if (files[file_count].name == NULL)
            {
                printf(RED "Error: Memory allocation failed\n" RESET);
                for (int i = 0; i < file_count; i++)
                    free(files[i].name);
                closedir(dir);
                return;
            }

            total_blocks += files[file_count].st.st_blocks;
            file_count++;
        }
        else if (errno == EACCES)
            printf(RED "Error: Permission denied for %s\n" RESET, entry->d_name);
        else
            printf(RED "Error: Cannot stat %s\n" RESET, entry->d_name);
    }

    closedir(dir);

    qsort(files, file_count, sizeof(FileInfo), compare);

    // Print total blocks used by the files
    if (l)
        printf("total %lld\n", total_blocks / 2); // Divide by 2 to convert from 512-byte blocks to 1024-byte blocks

    // Print file information
    for (int i = 0; i < file_count; i++)
    {
        char *name = files[i].name;
        struct stat *st = &files[i].st;

        if (l)
        {
            printFileColor(st);

            // Print number of blocks
            printf("%2ld ", st->st_blocks / 2); // Divide by 2 to convert from 512-byte blocks to 1024-byte blocks

            // Print file permissions
            printf((S_ISDIR(st->st_mode)) ? "d" : "-");
            printf((st->st_mode & S_IRUSR) ? "r" : "-");
            printf((st->st_mode & S_IWUSR) ? "w" : "-");
            printf((st->st_mode & S_IXUSR) ? "x" : "-");
            printf((st->st_mode & S_IRGRP) ? "r" : "-");
            printf((st->st_mode & S_IWGRP) ? "w" : "-");
            printf((st->st_mode & S_IXGRP) ? "x" : "-");
            printf((st->st_mode & S_IROTH) ? "r" : "-");
            printf((st->st_mode & S_IWOTH) ? "w" : "-");
            printf((st->st_mode & S_IXOTH) ? "x" : "-");

            // Print number of links
            printf(" %1ld ", st->st_nlink);

            // Print user and group names
            struct passwd *pw = getpwuid(st->st_uid);
            struct group *gr = getgrgid(st->st_gid);
            printf("%-9s %-9s ", pw ? pw->pw_name : "🤷", gr ? gr->gr_name : "🤷"); // 🤷 if user/group not found

            // Print file size
            printf("%5lld ", (long long)st->st_size);

            // Print last modified time
            char time_str[20];
            strftime(time_str, sizeof(time_str), "%b %d %H:%M", localtime(&st->st_mtime));
            printf("%s ", time_str);

            // Print file name
            printf("%s\n" RESET, name);
        }
        else
        {
            // Print file name coloured according to file type
            printFileColor(st);
            printf("%s\n" RESET, name);
        }

        free(files[i].name); // Free memory allocated by strdup
    }
}

void printFileColor(struct stat *st)
{
    if (S_ISDIR(st->st_mode))
        printf(BLUE);
    else if (st->st_mode & S_IXUSR)
        printf(GREEN);
    else
        printf(WHITE);
}

int compare(const void *a, const void *b)
{
    return strcmp(((FileInfo *)a)->name, ((FileInfo *)b)->name);
}