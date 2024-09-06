#include "processes.h"

processStruct *createProcessStruct(pid_t pid, char *name)
{
    processStruct *newProcess = (processStruct *)malloc(sizeof(processStruct));
    if (newProcess == NULL)
    {
        printf(RED "Error: Memory allocation failed for background process\n" RESET);
        return NULL;
    }

    newProcess->pid = pid;
    strcpy(newProcess->name, name);
    newProcess->next = NULL;

    return newProcess;
}

void addProcess(processStruct *newProcess, processStruct **head)
{
    // If the linked list is empty
    if (*head == NULL)
    {
        *head = newProcess;
        return;
    }

    // Add the process to the end of the linked list
    processStruct *temp = *head;
    while (temp->next != NULL)
        temp = temp->next;

    temp->next = newProcess;
}

void checkBgProcesses(struct processStruct **processList)
{
    processStruct *temp = *processList;
    processStruct *prev = NULL;

    while (temp != NULL)
    {
        int status;
        pid_t result = waitpid(temp->pid, &status, WNOHANG);
        if (result == -1)
            return;

        if (result > 0)
        {
            if (WIFEXITED(status))
                printf("%s exited normally (%d)\n", temp->name, temp->pid);
            else
                printf("%s exited abnormally (%d)\n", temp->name, temp->pid);

            // Remove the process from the linked list
            if (prev == NULL) // If the process is the head of the linked list
            {
                *processList = temp->next;
                free(temp);
                temp = *processList;
            }
            else
            {
                prev->next = temp->next;
                free(temp);
                temp = prev->next;
            }
        }
        else
        {
            prev = temp;
            temp = temp->next;
        }
    }
}

void freeProcesses(processStruct *head)
{
    processStruct *temp = head;
    while (temp != NULL)
    {
        kill(temp->pid, SIGKILL);
        processStruct *next = temp->next;
        free(temp);
        temp = next;
    }
}
