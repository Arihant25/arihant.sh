#ifndef IMAN_H
#define IMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ctype.h>

#include "colors.h"

#define PORT 80
#define BUFFER_SIZE 4096


// Strip HTML tags from the given HTML string
void strip_html_tags(char *html, char *text);

// Get the manual page for the given command from the internet
void iMan(const char *command);

#endif