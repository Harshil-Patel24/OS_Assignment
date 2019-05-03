#include "scheduler.h"

/* Public variables which are used by threads */
LinkedList* readyQ;
LinkedList* tasks;

FILE* simulationLog;

int numTasks;
int totalWaitingTime;
int totalTurnAroundTime;

int finish;

/* Mutexes declated here */
pthread_mutex_t qLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t readyQLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t cpuLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t cpuJoinLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t logLock = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t finished = PTHREAD_COND_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;



int counter = 0;

/* Scheduler.h will run the program and track most of its flow
 * it will have cpu() and task() methods */
int main( int argc, char** argv )
{
    /* Checks if correct number of arguments */
    if( argc == 3 )
    {
        /* Variable declarations/initialisations */
        int maxSize = 0;
        char* fileName = NULL;
        numTasks = 0;
        totalWaitingTime = 0;
        totalTurnAroundTime = 0;
        finish = 0;

        /* Third argument should be maximum queue sized required */
        if( atoi( argv[2] ) > 0 )
        {
            maxSize = atoi( argv[2] );
            readyQ = makeList( maxSize );
            fileName = argv[1];
            simulationLog = fopen( "simulation_log", "a" );

            if( simulationLog != NULL )
            {
                schedule( fileName );
                fclose( simulationLog );
            }
            else
            {
                perror( "Error: Could not open file: simulation_log to append\n" );
            }

            free( readyQ );
            free( tasks );
        }
        else
        {
            perror( "Error: Invalid argument\nUsage- ./scheduler <task file> <ready queue size>" );
        }
    }
    else
    {
        perror( "Error: Incorrect number of arguments\nUsage- ./scheduler <task file> <ready queue size>" );
    }

    return 0;
}

void schedule( char* taskFileName )
{
    int ii;
    int numLines = 0;
    /* CPU names for differenciating which thread does which task */
    char* cpuNums[3] = {"CPU-1","CPU-2","CPU-3"};

    /* Thread declarations */
    pthread_t cpuThread[3];
    pthread_t taskThread;

    /* Fill up all the tasks from the file to a linked list */
    numLines = getNumLines( taskFileName );
    tasks = makeList( numLines );
    fileToLL( tasks, taskFileName );

    /* Start threads */
    /* Start task thread so it can fill up readyQ */
    pthread_create( &taskThread, NULL, task, NULL );

    /* Start all three CPU threads */
    for( ii = 0; ii < 3; ii++ )
    {
        pthread_create( &cpuThread[ii], NULL, cpu, cpuNums[ii] );
    }

    /* Join threads */
    /* Join task thread */
    pthread_join( taskThread, NULL );

    pthread_mutex_lock(  &cpuJoinLock );
    if( finish < 3 )
    {
        pthread_cond_wait( &finished, &cpuJoinLock );
        printf( "Finish: %d\n",finish );
    }
    /* Join all three CPU threads */
    for( ii = 0; ii < 3; ii++ )
    {
        pthread_join( cpuThread[ii], NULL );
    }
}

/* Simulates a CPU completing a task */
void* cpu( void* inCPUNum )
{
    /* Variable declarations */
    int taskID, burstTime;
    int hr, min, sec, arriveTimeInt, serviceTimeInt, waitTime, turnAroundTime;
    Time serviceTime, arrivalTime;
    char logStr[150];
    /* Time related declarations */
    time_t timer;
    char servTime[50];
    char* cpuNum;
    struct tm* serviceTimeInfo;
    /* Represents the CPU number to differenciate which CPU has completed which task */
    cpuNum = ( char* )inCPUNum;


    pthread_mutex_lock( &readyQLock );
    while( !isEmpty( tasks ) /* || !isEmpty( readyQ ) */)
    { /*
printf("Tasks: %d\n", tasks->size );
printf("Ready Queue: %d\n", readyQ->size ); */
pthread_mutex_lock( &cpuLock );
        if( !isEmpty( readyQ ) )
        {
/*printf( "%s gained readyQLock\n", cpuNum );*/
            /* Take info from head of tasks linked list (ie. first task in queue) */
/*counter++;
printf("%d\n",counter);
*/          taskID = readyQ->head->task.taskID;
            burstTime = readyQ->head->task.burstTime;
            arrivalTime = readyQ->head->task.arrivalTime;
            /* pthread_mutex_unlock( &readyQLock ); */

            hr = arrivalTime.hour;
            min = arrivalTime.minute;
            sec = arrivalTime.second;

            /* This represents current time, ie. service time */
            time( &timer );
            serviceTimeInfo = localtime( &timer );

            /* Converts to string for ease of use */
            strftime( servTime, 50, "%H:%M:%S", serviceTimeInfo );

            /* Sleeps for time period, simulating CPU completing task */
            sleep( burstTime * 0.01 );

            /* Solve to the readers-writers problem */
            pthread_mutex_lock( &logLock );
/*printf( "%s gained logLock\n", cpuNum );*/

            /* Write to log */
            sprintf( logStr, "================================\nStatistics for %s:\nTask: %d\nArrivalTime: %02d:%02d:%02d\nService Time: %s\n", cpuNum, taskID, hr, min, sec, servTime );
            writeLog( simulationLog, logStr );
            pthread_mutex_unlock( &logLock );
/*printf( "%s released logLock\n", cpuNum );*/

            numTasks++;

            /* Calculate the wait time and turn around time */
            serviceTime.hour = serviceTimeInfo->tm_hour;
            serviceTime.minute = serviceTimeInfo->tm_min;
            serviceTime.second = serviceTimeInfo->tm_sec;

/*printf( "%s gained cpuLock\n", cpuNum );*/

            arriveTimeInt = timeToSec( arrivalTime );
            serviceTimeInt = timeToSec( serviceTime );
/*printf("Arrival time is: %d\nService time is: %d\n", arriveTimeInt, serviceTimeInt );
*/            waitTime = serviceTimeInt - arriveTimeInt;
            turnAroundTime = waitTime + burstTime;
/* printf("Wait time: %d\n", waitTime ); */
            totalWaitingTime += waitTime;
            totalTurnAroundTime += turnAroundTime;

            /* pthread_mutex_lock( &readyQLock ); */
            /* Once task is finished, remove it from queue */
            removeFirst( readyQ );
/*printf( "%s released readyQLock\n", cpuNum );*/
/*printf( "%s released cpuLock\n", cpuNum );*/
        }
        else
        {
            /* If ready queue is empty, block */
/*printf( "%s cond wait with qLock\n", cpuNum );*/
            pthread_cond_wait( &condition, &qLock );
        }
        pthread_mutex_unlock( &cpuLock );
/*        pthread_mutex_unlock( &readyQLock ); */
    }
    finish++;
/*    pthread_cond_signal( &finished );
*/
printf("CPU numTasks: %d\nTotal wait time: %d\nTotal turn around time: %d\n", numTasks, totalWaitingTime, totalTurnAroundTime );

/*
    if( finish >= 3  )
    { */
        pthread_cond_signal( &finished );
        printf( "Signalled with finish value of: %d\n", finish );
/*    } */

    return NULL;
}

/* Used code from https://stackoverflow.com/questions/3673226/how-to-print-time-in-format-2009-08-10-181754-811 used and modified to format time */
void* task( void* nothing )
{
    /* Variable declarations
     *This will store the string to be written to simulation_log */
    int ii;
    char logStr[50];

    /* Task info */
    int taskID, burstTime;

    /* Time related variables */
    int hr, min, sec;
    time_t timer;
    char currTime[50];
    struct tm* tmInfo;

    /* Keep looping and filling in any tasks that have been completed
     * Will loop until all tasks from the full task queue have been moved to ready queue */
    while( !isEmpty( tasks ) )
    {
        /* Protect the ready queue as it is a shared variable */
        /* Only fill the ready queue if it isnt already full */
        if( ( readyQ->size <= readyQ->max - 2 ) )
        {
            for( ii = 0; ii < 2; ii++ )
            {
                if( !isFull( readyQ ) )
                {
                    /* Get current time (to be used as arrival time) */
                    time( &timer );
                    tmInfo = localtime( &timer );

                    /* Store it in three variables so we can store in linked list
                     * with a Time attribute */
                    hr = tmInfo->tm_hour;
                    min = tmInfo->tm_min;
                    sec = tmInfo->tm_sec;

                    /* Take info from the first task in the tasks queue */
                    taskID = tasks->head->task.taskID;
                    burstTime = tasks->head->task.burstTime;

                    /*printf( "task() gained readyQLock\n" );*/
                    insertLast( readyQ, taskID, burstTime, hr, min, sec );
                    /*printf( "task() released readyQLock\n" );*/
                    strftime( currTime, 50, "%H:%M:%S", tmInfo );

                    /* Solve to readers-writers problem */
                    pthread_mutex_lock( &logLock );
                    /* printf( "task() gained logLock\n" ) */;
                    sprintf( logStr, "================================\n%d: %d\nArrival Time: %s\n", taskID, burstTime, currTime );
                    writeLog( simulationLog, logStr );
                    /* printf( "task() released logLock\n" ); */
                    pthread_mutex_unlock( &logLock );
                    removeFirst( tasks );
                    /*    } */
                }
            }
        }
        pthread_mutex_unlock( &readyQLock );
            /* If ready queue is full, unblock CPU */
        pthread_cond_signal( &condition );
    }
    /* pthread_mutex_unlock( &readyQLock );
*/
    return NULL;
}
