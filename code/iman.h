#ifndef IMAN_H
#define IMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "colors.h"

#define PORT 80
#define BUFFER_SIZE 4096

void iMan(const char *command);

#endif