# Arihant.sh

[![wakatime](https://wakatime.com/badge/user/77cdaa68-53d6-4cf6-8c9c-7ec147407ce9/project/7abeae12-6456-4d82-b8a1-1156bae4ef28.svg)](https://wakatime.com/badge/user/77cdaa68-53d6-4cf6-8c9c-7ec147407ce9/project/7abeae12-6456-4d82-b8a1-1156bae4ef28)

## Description 📃
This is a shell emulator that can run basic shell commands.
It was created as a part of the Operating Systems and Networks course at IIIT Hyderabad.

It is written in C and uses the system calls `fork()`, `execvp()`, `wait()`, as well as implements some basic shell commands such as `cd`, `ls`, and `find`.

## Technologies Used 💻
- C
- Unix System Calls
- Shell Commands
- Makefile

## How to Run 🚀
1. Clone the repository
2. Run `make` in the terminal.

## Features 🌟
- Run basic shell commands
- Execute multiple commands in a single line
- Run commands in the background
- Change directories
- View the log of commands run
- Output in full Technicolor!
- Customise the shell with a `.myshrc` file (aliases)
- Pipe commands
- Redirect input and output
- Exit the shell (no complicated steps required)

## Commands Supported 🪄
- hop (cd)
- reveal (ls)
- log (history)
- proclore
- seek (find)
- activities (jobs)
- ping (kill)
- fg
- bg
- neonate
- iMan (man)
- exit
- Any other shell command

## Usage 📝
1. Type `make` in the terminal to compile and run the code.
2. Use `make clean` to clean up the object and executable files, etc.

## Assumptions 🤔
This project was based on the guidelines mentioned [here](https://web.archive.org/web/20240906104046/https://karthikv1392.github.io/cs3301_osn/mini-projects/mp1).

Accordingly, I have made some assumptions where the guidelines were not clear.

1. The maximum string length is 4096 characters. This applies to length of a command, length of a path, etc.

2. The previous directory `-` is set the same as the home directory when the shell is started. The home directory is set to the directory where the shell is started.

3. When you run multiple commands in a single line, the prompt will show the total run time as the sum of the run times of all the commands.

4. The `reveal` command will colour the entire line when used with the `-l` flag.

5. When you run a command in the background, the shell will not wait for the command to finish. It will immediately return to the prompt. The command will run in the background. It will also not show the run time of the command in the prompt.

6. When `log execute` is used to run some command, the command won't show up in the prompt. Only `log` will show up in the prompt.

7. The `seek` command will search for files with the given name as a substring in the current directory and all its subdirectories. It will show the relative path of the files found.

8. If an erroneous command is run in the background, the shell will print an error message and return to the prompt. You might have to press enter to get the prompt back. This is because the shell will not wait for the command to finish.

9. The `proclore` command gives the virtual memory in KB used by the shell. Moreoever, some processes may require privileges to know the path to their executable. In such cases, the shell will not print the path to the executable.

10. The `iMan` command will show an error message if the command is not found in the manual.

11. The maximum number of aliases that can be stored is 4096.

12. The maximum number of pipes that can be used in a single command is 4096.

13. If a command run is run in the background, and the command is invalid, it will print a PID along with the error message. The PID will be the PID of the child process that was forked to run the command.

## Manual 📖
### hop
- `hop` is used to change the current working directory.
- Usage: `hop [directory]`

The locations you can use with `hop` are:
- `-` : Go to the previous directory
- `~` : Go to the home directory
- `.` : Stay in the current directory
- `..` : Go to the parent directory

You can provide multiple locations to `hop` in a single line.
For example, `hop .. ..` will take you to the grandparent directory.

### reveal
- `reveal` is used to list the contents of a directory.
- Usage: `reveal [directory]`

The flags you can use with `reveal` are:
- `-l` : Show details about each file
- `-a` : Show hidden files

### log
- `log` is used to view the history of commands run.

The arguments you can pass to `log` are:
- `execute <index>` : Run the command at the given index in the log, starting from the most recent command at index 1.
- `purge` : Clear the log of all commands.

### proclore
- `proclore` is used to view the virtual memory used by the shell.

By default it gives details about the shell process, but if passed a process ID, it will give details about that process.

### seek
- `seek` is used to search for files with a given name.

Usage : `seek <file_name> <directory>`

The directory is optional. If not provided, the search will be done in the current directory.

It recursively searches for files and folders with the given name as a substring.

The flags it accepts are:
- `-f` : Search only for files
- `-d` : Search only for directories
- `e` : If only one file is found, print it in the terminal. If only one folder is found, print the contents of the folder.

### activities
- `activities` is used to view a list of every process that was spawned by the shell in the current session.

### ping
- `ping` is used to send a signal to a process.

Usage : `ping <process_id> <signal>`

### fg
- `fg` is used to bring a background process to the foreground.

Usage : `fg <process_id>`

### bg
- `bg` is used to send a process to the background.

Usage : `bg <process_id>`

### neonate
- `neonate` is used to print the process ID of the most recently spawned process.

Usage : `neonate -n <number>`

### iMan
- `iMan` is used to view the manual of a command from the internet.

Usage : `iMan <command>`

### exit
- `exit` is used to exit the shell.

### Signals
- The shell supports the following signals:
  - `SIGINT` : Interrupt signal (Ctrl + C)
  - `SIGTSTP` : Stop signal (Ctrl + Z)
  - `eof` : End of file (Ctrl + D)

### Customisation
You can customise the shell by adding aliases to the `.myshrc` file in the home directory.

The format for adding aliases is:
```
alias <alias_name> = "<command>"
```

For example:
```
alias reveala = "reveal -a"
```

### Pipes
- The shell supports piping of commands.

For example:
```
reveal | grep .c
```

### Redirects
- The shell supports redirection of input and output using <, > and >>.

For example:
```
reveal > output.txt
```

```
reveal < input.txt
```

## Acknowledgements 🙏
- Linux Man Pages - For providing the documentation for the shell commands
- [Claude](https://1drv.ms/f/s!AiCXUjj-oBYdgsYUgTEwHKnuvzMFxw?e=4foifx) and [ChatGPT](https://justpaste.it/e0sf6) - For invaluable advice and guidance throughout the project
- OSN TA's - For coordinating the project and answering our queries
- [Prof. Karthik V](https://karthikv1392.github.io) - For giving us the opportunity to work on this project and providing the guidelines
- [IIIT Hyderabad](https://www.iiit.ac.in/) - For providing the platform and resources to work on this project
