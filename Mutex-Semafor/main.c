#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <getopt.h>
#include <stdbool.h>
#include <syslog.h>
#include <stdio.h>
#include "functions.h"

void *Client(void *arg);
void *Barber();

sem_t semClient;
sem_t semBarber;
pthread_mutex_t mutexSeat;
pthread_mutex_t mutexWRoom;

struct node *allClients = NULL;
//struct node *rejectedClients = NULL;
//struct node *clientsB4WRoom = NULL;
//struct node *clientsInWRoom = NULL;

int maxSeatsInWRoom = 10;
int numberOfClients = 30;
int freeSeatsInWRoom = 10;
int maxClippingTime = 10;
int maxClientArrivalTime = 10;
int rejectedCunter = 0;
bool bDebug = false;

int main(int argc, char **argv)
{
    openlog("SleepingBarber", LOG_CONS, LOG_USER);

    int option;
    while ((option = getopt(argc, argv, "c:s:t:m:d")) != -1)
        switch (option)
        {
        case 'c': //number of clients that will come to barber
            numberOfClients = atoi(optarg);
            break;
        case 's': //maximum number of seats in waiting room
            maxSeatsInWRoom = atoi(optarg);
            freeSeatsInWRoom = atoi(optarg);
            break;
        case 't': //maximum clipping time
            maxClippingTime = atoi(optarg);
            break;
        case 'm': //maximum difrence between arrival of clients
            maxClientArrivalTime = atoi(optarg);
            break;
        case 'd': //debug boolean
            bDebug = true;
            break;
        case '?':
            if (optopt == 'c')
                syslog(LOG_ERR, "Option -c requires argument");
            else if (optopt == 's')
                syslog(LOG_ERR, "Option -s requires argument");
            else if (optopt == 't')
                syslog(LOG_ERR, "Option -t requires argument");
            else if (optopt == 'm')
                syslog(LOG_ERR, "Option -m requires argument");
            else
                syslog(LOG_ERR, "Unknown option character");
            exit(EXIT_FAILURE);
            break;
        default:
            abort();
        }

    srand(time(NULL));

    pthread_t *clientThread = malloc(sizeof(pthread_t) * numberOfClients);
    pthread_t barberThread;

    //semaphore inicialization
    sem_init(&semBarber, 0, 0);
    sem_init(&semClient, 0, 0);

    for (int i = 0; i < numberOfClients; i++)
    {
        int timeOfArrival = rand() % maxClientArrivalTime + 1;
        Append(&allClients, i, timeOfArrival);
        pthread_create(&clientThread[i], NULL, Client, (void *)allClients);
    }
    /*Print_list(allClients);
    printf("\n");
    Remove(&allClients, 29);
    Print_list(allClients);
    printf("\n");*/
    pthread_create(&barberThread, NULL, Barber, NULL);

    if (pthread_mutex_init(&mutexSeat, NULL) != 0)
    {
        syslog(LOG_ERR, "Error creating mutexSeat");
        exit(EXIT_FAILURE);
    }
    if (pthread_mutex_init(&mutexWRoom, NULL) != 0)
    {
        syslog(LOG_ERR, "Error creating mutexSeat");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numberOfClients; i++)
    {
        pthread_join(clientThread[i], NULL);
    }

    pthread_join(barberThread, NULL);

    sem_destroy(&semBarber);
    sem_destroy(&semClient);
    pthread_mutex_destroy(&mutexSeat);
    pthread_mutex_destroy(&mutexWRoom);

    free(allClients);
    exit(EXIT_SUCCESS);
}

void *Client(void *arg) //Client thread
{
    return NULL;
}

void *Barber() //Barber thread
{
    return NULL;
}
