#include "iman.h"

void iMan(const char *command)
{
    int sockfd;                     // Socket file descriptor
    struct sockaddr_in server_addr; // Server address structure
    struct hostent *server;         // Server details
    char buffer[BUFFER_SIZE];       // Buffer for sending and receiving data

    // Create a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        printf(RED "Error: Couldn't open socket\n" RESET);
        return;
    }

    // Get host by name
    server = gethostbyname("man.he.net");
    if (server == NULL)
    {
        printf(RED "Error: Couldn't find the host man.he.net\n" RESET);
        return;
    }

    // Set up server address structure
    memset(&server_addr, 0, sizeof(server_addr));                                   // Zero out the structure
    server_addr.sin_family = AF_INET;                                               // Internet address family
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length); // Server IP address
    server_addr.sin_port = htons(PORT);                                             // Server port

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf(RED "Error: Couldn't connect to the server\n" RESET);
        return;
    }

    // Prepare GET request
    snprintf(buffer, BUFFER_SIZE,
             "GET /?topic=%s&section=all HTTP/1.1\r\n"
             "Host: man.he.net\r\n"
             "User-Agent: iMan/1.0\r\n"
             "Accept: text/html\r\n"
             "Connection: close\r\n\r\n",
             command);

    // Send GET request
    if (send(sockfd, buffer, strlen(buffer), 0) < 0)
    {
        printf(RED "Error: Couldn't send the request\n" RESET);
        return;
    }

    size_t total_received = 0;
    char response[1000000] = {0}; // Allocate 1MB for the response

    // Receive response
    while (true)
    {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0)
            break;
        memcpy(response + total_received, buffer, bytes_received);
        total_received += bytes_received;
    }

    printf(BOLD "Manual page for %s:\n" RESET, command);

    // Handle missing man pages
    char *missing_str = (char *)malloc(4096);
    snprintf(missing_str, 4096, "<h2 align=center>No matches for \"%s\"", command);
    if (strstr(response, missing_str))
    {
        printf(RED "No manual entry for \"%s\"\n" RESET, command);
        return;
    }

    // Find the start of the content
    char *content_start = strstr(response, "<html");
    if (content_start)
        printf("%s", content_start);
    else
        printf("Could not find start of content. Printing entire response:\n%s", response);

    close(sockfd);
}