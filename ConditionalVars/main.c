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


//----------------------------------------------------------------Mutexs declaration
pthread_mutex_t mutexWRoom;
pthread_mutex_t mutexBarber;


//----------------------------------------------------------------Conditional declaration
pthread_cond_t barberSleeping;


//----------------------------------------------------------------Initialization of linked lists
struct node *allClients = NULL;
struct node *resignedClients = NULL;
struct node *clientsInWRoom = NULL;


//----------------------------------------------------------------Initialization of variables
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
    openlog("SleepingBarber", LOG_CONS, LOG_USER);

    //------------------------------------------------------------Getting values from options
    int option;
    while ((option = getopt(argc, argv, "c:s:t:m:d")) != -1)
        switch (option)
        {
        case 'c': //----------------------------------------------Number of clients that will come to barber
            numberOfClients = atoi(optarg);
            break;
        case 's': //----------------------------------------------Maximum number of seats in waiting room
            maxSeatsInWRoom = atoi(optarg);
            freeSeatsInWRoom = atoi(optarg);
            break;
        case 't': //----------------------------------------------Maximum clipping time
            maxClippingTime = atoi(optarg);
            break;
        case 'm': //----------------------------------------------Maximum difrence between arrival of clients
            maxClientArrivalTime = atoi(optarg);
            break;
        case 'd': //----------------------------------------------Debug boolean
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


    //------------------------------------------------------------Initializations
    if (pthread_mutex_init(&mutexWRoom, NULL) != 0)
    {
        syslog(LOG_ERR, "Error creating mutexWRoom");
        exit(EXIT_FAILURE);
    }
    if (pthread_mutex_init(&mutexBarber, NULL) != 0)
    {
        syslog(LOG_ERR, "Error creating mutexBarber");
        exit(EXIT_FAILURE);
    }
    if (pthread_cond_init(&barberSleeping, NULL) != 0)
    {
        syslog(LOG_ERR, "Error creating conditional barberSleeping");
        exit(EXIT_FAILURE);
    }


    //------------------------------------------------------------Creating threads
    pthread_t barberThread;
    pthread_t *clientThread = malloc(sizeof(pthread_t) * numberOfClients);


    int iret;
    iret = pthread_create(&barberThread, NULL, Barber, NULL);
    if (iret)
    {
        syslog(LOG_ERR, "Error creating barberThread");
        exit(EXIT_FAILURE);
    }


    for (long i = 0; i < numberOfClients; i++)
    {
        int timeOfArrival = rand() % maxClientArrivalTime + 1;
        Append(&allClients, i, timeOfArrival);
        iret = pthread_create(&clientThread[i], NULL, Client, (void *)i);
        if (iret)
        {
            syslog(LOG_ERR, "Error creating clientThread");
            exit(EXIT_FAILURE);
        }
    }


    //------------------------------------------------------------Joining threads
    for (int i = 0; i < numberOfClients; i++)
    {
        pthread_join(clientThread[i], NULL);
    }

    finished = true;

    pthread_join(barberThread, NULL);


    //------------------------------------------------------------Freeing memory
    pthread_mutex_destroy(&mutexWRoom);
    pthread_mutex_destroy(&mutexBarber);

    free(allClients);
    exit(EXIT_SUCCESS);
}

void *Client(void *cNumber) //------------------------------------Client thread
{
    long temp = (long)cNumber;
    int clientNumber = (int)temp;
    int travelTime = SleepTime(&allClients, clientNumber);
    sleep(travelTime);

    //------------------------------------------------------------Client enters building
    pthread_mutex_lock(&mutexWRoom);

    //------------------------------------------------------------Queue is full
    if (freeSeatsInWRoom <= 0)
    {
        //--------------------------------------------------------Leaving the building
        resignedCounter++;
        printf("Res:%d WRoom: %d/%d [in: %d]\t  Number %d resigned\n", resignedCounter, maxSeatsInWRoom - freeSeatsInWRoom, maxSeatsInWRoom, clientOnSeat, clientNumber);
        pthread_mutex_unlock(&mutexWRoom);
        if (bDebug == true)
        {
            Append(&resignedClients, clientNumber, 0);
            Print(resignedClients, clientsInWRoom);
        }
    }
    //------------------------------------------------------------Queue isn't full
    else
    {
        //--------------------------------------------------------Taking a seat in the queue
        freeSeatsInWRoom--;
        Append(&clientsInWRoom, clientNumber, 0);
        printf("Res:%d WRoom: %d/%d [in: %d]\t  Number %d joined queue\n", resignedCounter, maxSeatsInWRoom - freeSeatsInWRoom, maxSeatsInWRoom, clientOnSeat, clientNumber);
        if (bDebug == true)
        {
            Print(resignedClients, clientsInWRoom);
        }
        pthread_mutex_unlock(&mutexWRoom);

        while(clientsInWRoom->clientNumber != clientNumber) {}

        //--------------------------------------------------------Entering barber's room
        pthread_mutex_lock(&mutexBarber);
        
        //--------------------------------------------------------Leaving queue
        pthread_mutex_lock(&mutexWRoom);
        freeSeatsInWRoom++;
        clientOnSeat = clientNumber;
        pthread_mutex_unlock(&mutexWRoom);

        pthread_mutex_unlock(&mutexBarber);

        //--------------------------------------------------------Waking up the barber
        pthread_cond_signal(&barberSleeping);
        
        Pop(&clientsInWRoom);
        
    }
    return NULL;
}

void *Barber() //-------------------------------------------------Barber thread
{
    int clippingTime;
    pthread_mutex_lock(&mutexBarber);
    while (finished == false)
    {
        //--------------------------------------------------------Going to sleep
        pthread_cond_wait(&barberSleeping, &mutexBarber);

        //--------------------------------------------------------Clipping hair
        clippingTime = rand() % maxClippingTime + 1;
        sleep(clippingTime);
        printf("Res:%d WRoom: %d/%d [in: %d]\t  Number %d has been serviced\n", resignedCounter, maxSeatsInWRoom - freeSeatsInWRoom, maxSeatsInWRoom, clientOnSeat, clientOnSeat);
        if (bDebug == true)
        {
            Print(resignedClients, clientsInWRoom);
        }

        //--------------------------------------------------------Clearing the seat
        clientOnSeat = -1;
    }
    return NULL;
}
