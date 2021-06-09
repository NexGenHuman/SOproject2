#ifndef FUNCTIONS_H
#define FUNCTIONS_H

extern struct node {
    int clientNumber;
    struct node * next;
} node_t;

//Prints current state of client decisions np. Res:2 WRomm: 5/10 [in: 4]
void PrintState(int, int, int);
//Adds client to the list
void AddClient();
//Not sure what it's supposed to do
void AddClients();
void Print_list(node_t);
void Push(struct node *head, int cNumber);
void Remove(struct node *head, int cNumber);
void Append(struct node *head, int cNumber);

#endif