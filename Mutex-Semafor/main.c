#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <getopt.h>
#include <stdbool.h>
#include <syslog.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "functions.h"

void *Client(void *arg);
void *Barber();

sem_t semClient;
sem_t semBarber;
//sem_t semAccessWRoomSeats;
pthread_mutex_t mutexSeat;
pthread_mutex_t mutexWRoom;

struct node *allClients = NULL;
struct node *resignedClients = NULL;
//struct node *clientsB4WRoom = NULL;
struct node *clientsInWRoom = NULL;

int maxSeatsInWRoom = 10;
int numberOfClients = 30;
int freeSeatsInWRoom = 10;
int maxClippingTime = 6;
int maxClientArrivalTime = 30;
int resignedCounter = 0;
int clientOnSeat = -1;
bool bDebug = false;
bool finished = false;

int main(int argc, char **argv)
{
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
        case 'm': //maximum arrival time of clients
            maxClientArrivalTime = atoi(optarg);
            break;
        case 'd': //debug boolean
            bDebug = true;
            break;
        case '?':
            if (optopt == 'c')
                printf("Option -c requires argument");
            else if (optopt == 's')
                printf("Option -s requires argument");
            else if (optopt == 't')
                printf("Option -t requires argument");
            else if (optopt == 'm')
                printf("Option -m requires argument");
            else
                printf("Unknown option character");
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
    //sem_init(&semAccessWRoomSeats, 0, 1);
    int iret;
    for (long i = 0; i < numberOfClients; i++)
    {
        int timeOfArrival = rand() % maxClientArrivalTime + 1;
        Append(&allClients, i, timeOfArrival);
        iret = pthread_create(&clientThread[i], NULL, Client, (void *)i);
        if (iret)
        {
            printf("Error creating clientThread");
            exit(EXIT_FAILURE);
        }
        //printf("Created client");
    }
    /*Print_list(allClients);
    printf("\n");
    Remove(&allClients, 29);
    Print_list(allClients);
    printf("\n");*/
    iret = pthread_create(&barberThread, NULL, Barber, NULL);
    //printf("Created barber");
    if (iret)
    {
        printf("Error creating barberThread");
        exit(EXIT_FAILURE);
    }

    if (pthread_mutex_init(&mutexSeat, NULL) != 0)
    {
        printf("Error creating mutexSeat");
        exit(EXIT_FAILURE);
    }
    if (pthread_mutex_init(&mutexWRoom, NULL) != 0)
    {
        printf("Error creating mutexWRoom");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numberOfClients; i++)
    {
        pthread_join(clientThread[i], NULL);
    }

    finished = true;

    pthread_join(barberThread, NULL);

    sem_destroy(&semBarber);
    sem_destroy(&semClient);
    pthread_mutex_destroy(&mutexSeat);
    pthread_mutex_destroy(&mutexWRoom);

    free(allClients);
    exit(EXIT_SUCCESS);
}

void *Client(void *cNumber) //Client thread
{
    long temp = (long)cNumber;
    int clientNumber = (int)temp;
    int travelTime = SleepTime(&allClients, clientNumber);
    //printf("nr: %d traveltime: %d\n", clientNumber, travelTime);
    sleep(travelTime);
    pthread_mutex_lock(&mutexWRoom);

    if (freeSeatsInWRoom <= 0)
    {
        resignedCounter++;
        printf("Res:%d WRoom: %d/%d [in: %d] - Client: %d Resigned\n", resignedCounter, maxSeatsInWRoom - freeSeatsInWRoom, maxSeatsInWRoom, clientOnSeat, clientNumber);
        pthread_mutex_unlock(&mutexWRoom);
        if (bDebug == true)
        {
            Append(&resignedClients, clientNumber, 0);
            Print(resignedClients, clientsInWRoom);
        }
    }
    else
    {
        freeSeatsInWRoom--;
        if (bDebug == true)
        {
            Append(&clientsInWRoom, clientNumber, 0);
        }
        printf("Res:%d WRoom: %d/%d [in: %d] - Client: %d Joined queue\n", resignedCounter, maxSeatsInWRoom - freeSeatsInWRoom, maxSeatsInWRoom, clientOnSeat, clientNumber);
        if (bDebug == true)
        {
            Print(resignedClients, clientsInWRoom);
        }
        sem_post(&semClient);
        pthread_mutex_unlock(&mutexWRoom);
        sem_wait(&semBarber);
        
        pthread_mutex_lock(&mutexSeat);
        if (bDebug == true)
        {
            Remove(&clientsInWRoom, clientNumber);
        }
        clientOnSeat = clientNumber;
        printf("Res:%d WRoom: %d/%d [in: %d] - Client: %d Going for clipping\n", resignedCounter, maxSeatsInWRoom - freeSeatsInWRoom, maxSeatsInWRoom, clientOnSeat, clientNumber);
        if (bDebug == true)
        {
            Print(resignedClients, clientsInWRoom);
        }
    }
    return NULL;
}

void *Barber() //Barber thread
{
    int clippingTime;
    while (finished == false)
    {
        sem_wait(&semClient);
        pthread_mutex_lock(&mutexWRoom);
        sem_post(&semBarber);
        freeSeatsInWRoom++;
        pthread_mutex_unlock(&mutexWRoom);
        
        clippingTime = rand() % maxClippingTime + 1;
        sleep(clippingTime);
        pthread_mutex_unlock(&mutexSeat);
        clientOnSeat = -1;
        /*printf("Res:%d WRoom: %d/%d [in: %d]\n", resignedCounter, maxSeatsInWRoom - freeSeatsInWRoom, maxSeatsInWRoom, clientOnSeat);
        if (bDebug == true)
        {
            Print(resignedClients, clientsInWRoom);
        }*/
    }
    return NULL;
}
