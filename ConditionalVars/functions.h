#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>

extern struct node {
    int clientNumber;
    unsigned int time;
    struct node * next;
} node_t;

//Prints current state of client decisions np. Res:2 WRomm: 5/10 [in: 4]
//Parameters: How many resigned, How many in queue, Max number in queue, Client currently being serviced
void PrintState(int res, int wRoom, int roomCap, int client);
//Adds client to the list
void AddClient();
//Not sure what it's supposed to do
void AddClients();
void Print_list(node_t);
void Push(struct node_t *head, int cNumber, int timeOfArrival);
void Remove(struct node_t *head, int cNumber);
void Append(struct node_t *head, int cNumber, int timeOfArrival);

#endif