#include "functions.h"
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <getopt.h>
#include <stdbool.h>
#include <syslog.h>
#include <stdio.h>

//struct node *allClients;
//struct node *resignedClients;
//struct node *clientsB4WRoom;
//struct node *clientsInWRoom;

void Print_list(struct node *head)
{
    struct node *current = head;
    while (current != NULL)
    {
        printf(" %d", current->clientNumber);
        current = current->next;
    }
}

void Push(struct node **head, int cNumber, int _timeOfArrival)
{
    struct node *new_node;
    new_node = (struct node *)malloc(sizeof(struct node));
    new_node->clientNumber = cNumber;
    new_node->timeOfArrival = _timeOfArrival;
    new_node->next = *head;
    *head = new_node;
}

void Remove(struct node **head, int cNumber)
{
    struct node *current = *head;
    struct node *temp = NULL;
    if (current->clientNumber != cNumber)
    {
        while (current->next->clientNumber != cNumber)
        {
            current = current->next;
        }
        temp = current->next;
        current->next = temp->next;
        free(temp);
    }
    else
    {
        Pop(head);
    }
}

int Pop(struct node **head)
{
    struct node *next_node = NULL;
    if (*head == NULL)
        return -1;

    next_node = (*head)->next;
    free(*head);
    *head = next_node;
    return 0;
}

void Append(struct node **head, int cNumber, int _timeOfArrival)
{
    if (*head != NULL)
    {
        struct node *current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = (struct node *)malloc(sizeof(struct node));
        current->next->clientNumber = cNumber;
        current->next->timeOfArrival = _timeOfArrival;
        current->next->next = NULL;
    }
    else
    {
        struct node *new_node;
        new_node = (struct node *)malloc(sizeof(struct node));
        new_node->clientNumber = cNumber;
        new_node->timeOfArrival = _timeOfArrival;
        new_node->next = *head;
        *head = new_node;
    }
}

int SleepTime(struct node **head, int cNumber)
{
    struct node *current = *head;
    struct node *temp = NULL;

    while (current->clientNumber != cNumber)
    {
        current = current->next;
    }
    return current->timeOfArrival;
}

void Print(struct node *list1, struct node *list2)
{
    printf("DEBUG: Res:");
    Print_list(list1);
    printf(" WRoom:");
    Print_list(list2);
    printf("\n");
}