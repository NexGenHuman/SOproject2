#include "functions.h"

struct node_t *allClients;
struct node_t *rejectedClients;
struct node_t *clientsB4WRoom;
struct node_t *clientsInWRoom;

void printState(int res, int wRoom, int roomCap, int client)
{
    fprintf(stdout, "Res:%d WRomm: %d/%d [in: %d]", res, wRoom, roomCap, client);
}

void AddClients()
{

}

void AddClient()
{
    
}

void Print_list(node_t)
{

}

void Push(struct node_t *head, int cNumber, int timeOfArrival)
{
    struct node *newElement = (struct node*)malloc(sizeof(struct node));
    newElement->clientNumber = cNumber;
    newElement->time = timeOfArrival;
    newElement->next = head;
    head = newElement;
}

void Remove(struct node_t *head, int cNumber)
{
    if(head != NULL)
    {

    }
}

void Append(struct node_t *head, int cNumber, int timeOfArrival)
{

}