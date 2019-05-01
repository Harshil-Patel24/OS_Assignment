#include "scheduler.h"

/* Public variables which are used by threads */
LinkedList* readyQ;
LinkedList* tasks;

FILE* simulationLog;

int numTasks;
int totalWaitingTime;
int totalTurnAroundTime;

/* Mutexes declated here */
pthread_mutex_t qLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t cpuLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t logLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;

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

        /* Third argument should be maximum queue sized required */
        if( atoi( argv[2] ) > 0 )
        {
            maxSize = atoi( argv[2] );
            readyQ = makeList( maxSize );
            fileName = argv[1];
            simulationLog = fopen( "simulation_log", "a" );
            schedule( fileName );

            fclose( simulationLog );
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

    /* THIS WILL PROBABLY HAVE TO CHANGE
     * MAKE THIS OPEN AND APPEND INSTEAD */
    FILE* logFile = fopen( "simulation_log", "w" );

    /* Thread declarations */
    pthread_t cpuThread[3];
    pthread_t taskThread;

    if( logFile != NULL )
    {
        /* Fill up all the tasks from the file to a linked list */
        numLines = getNumLines( taskFileName );
        tasks = makeList( numLines );

        fileToLL( tasks, taskFileName );

        /* Start threads */
        pthread_create( &taskThread, NULL, task, NULL );

        for( ii = 0; ii < 3; ii++ )
        {
            pthread_create( &cpuThread[ii], NULL, cpu, cpuNums[ii] );
        }

        /* Join threads */
        pthread_join( taskThread, NULL );

        for( ii = 0; ii < 3; ii++ )
        {
            pthread_join( cpuThread[ii], NULL );
        }

    }
    else
    {
        perror( "Error: Couldn't open logfile to write\n" );
    }

}

/* Simulates a CPU completing a task */
void* cpu( void* inCPUNum )
{
    /* Variable declarations */
    int taskID, burstTime;
    int hr, min, sec, arriveTimeInt, serviceTimeInt, waitTime, turnAroundTime;
    Time serviceTime;
    char logStr[150];
    /* Time related declarations */
    time_t timer;
    char servTime[50];
    char* cpuNum;
    struct tm* serviceTimeInfo;
    /* Represents the CPU name */
    cpuNum = ( char* )inCPUNum;

    while( !isEmpty( readyQ ) || !isEmpty( tasks ) )
    {
 printf("CPU unlocked\n");
/*        while( )
        pthread_cond_wait( &condition, &lock );
*/
        pthread_mutex_lock( &cpuLock );
        pthread_mutex_lock( &logLock );
        /* Take info from head of tasks linked list (ie. first task in queue) */
        taskID = readyQ->head->task.taskID;
        burstTime = readyQ->head->task.burstTime;
        hr = readyQ->head->task.arrivalTime.hour;
        min = readyQ->head->task.arrivalTime.minute;
        sec = readyQ->head->task.arrivalTime.second;

        /* This represents current time, ie. service time */
        time( &timer );
        serviceTimeInfo = localtime( &timer );

        /* Converts to string for ease of use */
        strftime( servTime, 50, "%H:%M:%S", serviceTimeInfo );

        /* Sleeps for time period, simulating CPU completing task */
        sleep( burstTime * 0.01 );

        /* Write to log */
        sprintf( logStr, "================================\nStatistics for %s:\nTask: %d\nArrivalTime: %02d:%02d:%02d\nService Time: %s\n", cpuNum, taskID, hr, min, sec, servTime );
        writeLog( simulationLog, logStr );

        /* Calculate the wait time and turn around time */
        serviceTime.hour = serviceTimeInfo->tm_hour;
        serviceTime.minute = serviceTimeInfo->tm_min;
        serviceTime.second = serviceTimeInfo->tm_sec;

        arriveTimeInt = timeToSec( readyQ->head->task.arrivalTime );
        serviceTimeInt = timeToSec( serviceTime );

        waitTime = serviceTimeInt - arriveTimeInt;
        turnAroundTime = waitTime + burstTime;

        totalWaitingTime += waitTime;
        totalTurnAroundTime += turnAroundTime;

        /* Once task is finished, remove it from queue */
        removeFirst( readyQ );
        numTasks--;

        pthread_mutex_unlock( &logLock );
        pthread_mutex_unlock( &cpuLock );
/*        pthread_cond_signal( &condition );
*/
    }
    printf("CPU numTasks: %d\nTotal wait time: %d\nTotal turn around time: %d\n", numTasks, totalWaitingTime, totalTurnAroundTime );

    return NULL;
}

/* Used code from https://stackoverflow.com/questions/3673226/how-to-print-time-in-format-2009-08-10-181754-811 to format time */
void* task( void* nothing )
{
    /* Variable declarations */
    int ii;
    char logStr[50];

    /* Time related variables */
    int taskID, burstTime, hr, min, sec;
    time_t timer;
    char currTime[50];
    struct tm* tmInfo;

    /* Keep looping and filling in any tasks that have been completed */
    while( !isEmpty( tasks ) )
    {
/* printf( "Tasks unlocked\n" );
*/
        pthread_mutex_lock( &cpuLock );
        pthread_mutex_lock( &logLock );
        if( readyQ->size <= readyQ->max - 2 )
        {
/* printf("size < max - 2\n");
*/
            for( ii = 0; ii < 2; ii++ )
            {
                if( !isFull( readyQ ) )
                {
                    if( !isEmpty( tasks ) )
                    {
                        time( &timer );
                        tmInfo = localtime( &timer );

                        hr = tmInfo->tm_hour;
                        min = tmInfo->tm_min;
                        sec = tmInfo->tm_sec;

                        taskID = tasks->head->task.taskID;
                        burstTime = tasks->head->task.burstTime;

                        insertLast( readyQ, taskID, burstTime, hr, min, sec );

                        strftime( currTime, 50, "%H:%M:%S", tmInfo );

                        sprintf( logStr, "================================\n%d: %d\nArrival Time: %s\n", taskID, burstTime, currTime );
                        numTasks++;
                        writeLog( simulationLog, logStr );
                        removeFirst( tasks );
/* printf("Task numTasks: %d\n", numTasks );
*/
                    }
                }
            }
            /* pthread_cond_signal( &condition ); */
        }
        pthread_mutex_unlock( &logLock );
        pthread_mutex_unlock( &cpuLock );
    }
    return NULL;
}
