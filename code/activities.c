#include "activities.h"

void activities(struct processStruct *allProcesses)
{
    // Sort the processes in lexicographic order
    processStruct *temp = allProcesses;
    int num_processes = 0;
    while (temp != NULL)
    {
        num_processes++;
        temp = temp->next;
    }

    if (num_processes == 0)
    {
        printf("No processes to show\n");
        return;
    }

    char processNames[num_processes][4096];
    int processPids[num_processes];

    temp = allProcesses;
    for (int i = 0; i < num_processes; i++)
    {
        strcpy(processNames[i], temp->name);
        processPids[i] = temp->pid;
        temp = temp->next;
    }

    // Sort the processes (bubble sort)
    for (int i = 0; i < num_processes; i++)
        for (int j = i + 1; j < num_processes; j++)
            if (strcmp(processNames[i], processNames[j]) > 0)
            {
                // Swap the names
                char tempName[4096];
                strcpy(tempName, processNames[i]);
                strcpy(processNames[i], processNames[j]);
                strcpy(processNames[j], tempName);

                // Swap the PIDs
                int tempPid = processPids[i];
                processPids[i] = processPids[j];
                processPids[j] = tempPid;
            }

    // Print the processes
    for (int i = 0; i < num_processes; i++)
    {
        printf("%d : %s - ", processPids[i], processNames[i]);

        int status;
        pid_t result = waitpid(processPids[i], &status, WNOHANG);
        if (result == -1)
            printf(RED "Stopped\n" RESET);
        else
            printf(GREEN "Running\n" RESET);
    }
}