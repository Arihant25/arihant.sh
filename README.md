# Arihant.sh

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
- Can run basic shell commands
- Can run multiple commands in a single line
- Can run commands in the background
- Can change directories
- Can view the log of commands run
- Can output in full Technicolor!
- Can exit the shell (no complicated steps required)

## Commands Supported 🪄
- hop (cd)
- reveal (ls)
- log (history)
- proclore
- seek (find)
- exit
- Any other shell command

## Future Work 🚧
- Implement more shell commands
- Implement piping and redirection

## Assumptions 🤔
This project was based on the guidelines mentioned [here](https://web.archive.org/web/20240829135340/https://karthikv1392.github.io/cs3301_osn/mini-projects/mp1).

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
