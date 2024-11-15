#include "activities.h"

void activities(struct processStruct *allProcesses)
{
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
    char processStatus[num_processes][4096];

    temp = allProcesses;
    for (int i = 0; i < num_processes; i++)
    {
        strcpy(processNames[i], temp->name);
        processPids[i] = temp->pid;
        strcpy(processStatus[i], temp->status);
        temp = temp->next;
    }

    // Sort the processes in lexicographic order (bubble sort)
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

                // Swap the status
                char tempStatus[4096];
                strcpy(tempStatus, processStatus[i]);
                strcpy(processStatus[i], processStatus[j]);
                strcpy(processStatus[j], tempStatus);
            }

    // Print the processes
    bool print = false;
    for (int i = 0; i < num_processes; i++)
    {
        if (strcmp(processStatus[i], "Stopped") == 0)
        {
            printf("%d : %s - ", processPids[i], processNames[i]);
            printf(RED "Stopped\n" RESET);
            print = true;
        }
        else
        {
            int status;
            pid_t result = waitpid(processPids[i], &status, WNOHANG);
            if (result == 0)
            {
                printf("%d : %s - ", processPids[i], processNames[i]);
                if (WIFEXITED(status) || WIFSIGNALED(status))
                    printf(RED "Stopped\n" RESET);
                else
                printf(GREEN "Running\n" RESET);
                print = true;
            }
        }
    }

    if (!print)
        printf("No processes to show\n");
}