#ifndef FUNCTIONS_H
#define FUNCTIONS_H

//Linked list for storing clients
extern struct node {
    int clientNumber;
    int timeOfArrival;
    struct node * next;
} node;


//Prints numbers of all clients within a list
void Print_list(struct node *head);
//Pushes client into the first place in list
void Push(struct node **head, int cNumber, int _timeOfArrival);
//Removes client with a given number from the list
void Remove(struct node **head, int cNumber);
//Removes first client from the list, then retuns his number
int Pop(struct node **head);
//Adds client to the end of a list
void Append(struct node **head, int cNumber, int _timeOfArrival);
//Returns time of arrival to the barber of a chosen client
int SleepTime(struct node **head, int cNumber);
//Prints debug message
void Print(struct node *list1, struct node *list2);

#endif