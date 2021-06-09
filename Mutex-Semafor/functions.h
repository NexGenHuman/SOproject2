#ifndef FUNCTIONS_H
#define FUNCTIONS_H

extern struct node {
    int clientNumber;
    int timeOfArrival;
    struct node * next;
} node;

//Prints current state of client decisions np. Res:2 WRomm: 5/10 [in: 4]
void PrintState(int, int, int);
void Print_list(struct node *head);
void Push(struct node **head, int cNumber, int _timeOfArrival);
void Remove(struct node **head, int cNumber);
int Pop(struct node **head);
void Append(struct node **head, int cNumber, int _timeOfArrival);
int SleepTime(struct node **head, int cNumber);
void Print(struct node *list1, struct node *list2);

#endif