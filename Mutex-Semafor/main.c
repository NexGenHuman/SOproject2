#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <getopt.h>
#include <stdbool.h>
#include <syslog.h>

sem_t semClient;
sem_t semBarber;
pthread_mutex_t mutexSeat;
pthread_mutex_t mutexWRoom;

int maxSeatsInWRoom = 10;
int NumberOfClients = 30;
int freeSeatsInWRoom = 10;
int maxClippingTime = 10;
int maxClientArrivalTimeDiffrence = 10;
bool bDebug = false;

int main(int argc, char **argv)
{
    openlog("SleepingBarber", LOG_CONS, LOG_USER);
    int option;
    while ((option = getopt(argc, argv, "c:s:t:m:d")) != -1)
        switch (option)
        {
        case 'c': //number of clients that will come to barber
            NumberOfClients = optarg;
            break;
        case 's': //maximum number of seats in waiting room
            maxSeatsInWRoom = optarg;
            break;
        case 't': //maximum clipping time
            maxClippingTime = optarg;
            break;
        case 'm': //opcja "-s sleeptime" - zmienia czas spania
            maxClientArrivalTimeDiffrence = optarg;
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

    

    exit(EXIT_SUCCESS);
}