#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <getopt.h>
#include <stdbool.h>
#include <syslog.h>
#include "functions.h"

void *CThread(void *arg) {}

sem_t semClient;
sem_t semBarber;
pthread_mutex_t mutexSeat;
pthread_mutex_t mutexWRoom;

struct node_t *allClients = NULL;
struct node_t *rejectedClients = NULL;
struct node_t *clientsB4WRoom = NULL;
struct node_t *clientsInWRoom = NULL;

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
            numberOfClients = optarg;
            break;
        case 's': //maximum number of seats in waiting room
            maxSeatsInWRoom = optarg;
            freeSeatsInWRoom = optarg;
            break;
        case 't': //maximum clipping time
            maxClippingTime = optarg;
            break;
        case 'm': //maximum difrence between arrival of clients
            maxClientArrivalTime = optarg;
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

    pthread_t clientThread[numberOfClients];
    pthread_t barberThread;
    pthread_t addClientThread;

    //semaphore inicialization
    sem_init(&semClient, 0, 0);
    sem_init(&semBarber, 0, 0);

    for (int i = 0; i < numberOfClients; i++)
    {
        //miejsce na dodanie klienta do listy
        int timeOfArrival = rand() % maxClientArrivalTime + 1;
        append(allClients, i, timeOfArrival);
        pthread_create(&clientThread[i], NULL, Client, (void *)i);
    }

    pthread_create(&barberThread, NULL, Barber, NULL);

    pthread_create(&addClientThread, NULL, AddClients, NULL);

    for (int i = 0; i < numberOfClients; i++)
    {
        pthread_join(clientThread[i], NULL);
    }

    pthread_join(barberThread, NULL);

    sem_destroy(&semBarber);
    sem_destroy(&semClient);
    pthread_mutex_destroy(&mutexSeat);
    pthread_mutex_destroy(&mutexWRoom);

    exit(EXIT_SUCCESS);
}

void *Client(void *arg) //Client thread
{
}

void *Barber(void *arg) //Barber thread
{
}
