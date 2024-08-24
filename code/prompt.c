#include "prompt.h"

void prompt(const char *home_dir)
{
    char hostname[4096], cwd[4096], username[4096];

    struct passwd *p;
    struct utsname u;

    uname(&u);                   // Get the system name into the utsname struct
    gethostname(hostname, 4096); // Get the hostname with buffer size 4096
    getcwd(cwd, sizeof(cwd));    // Get the current working directory

    p = getpwuid(getuid()); // Get the username
    if (p == NULL)          // If the username is not found
    {
        perror("Could not get username");
        return;
    }

    if (strcmp(cwd, home_dir) == 0) // If the current working directory is the home directory
        printf("\033[1;33m<%s@%s:~> \033[0m", p->pw_name, u.nodename);

    else if (strstr(cwd, home_dir) != NULL) // If cwd is a sub-directory of the home directory
        printf("\033[1;33m<%s@%s:~%s> \033[0m", p->pw_name, u.nodename, cwd + strlen(home_dir));

    else
        printf("\033[1;33m<%s@%s:%s> \033[0m", p->pw_name, u.nodename, cwd);

    return;
}