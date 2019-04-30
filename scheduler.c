#include "scheduler.h"

/* Public variables which are used by threads */
LinkedList* readyQ;
LinkedList* tasks;

int numTasks;
int totalWaitingTime;
int totalTurnAroundTime;

/* Mutexes declated here */
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
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
/*            if( pthread_mutex_init( &lock, NULL ) == 0 )
            {
*/
                maxSize = atoi( argv[2] );
                readyQ = makeList( maxSize );
                fileName = argv[1];
                schedule( fileName );
/*            }
            else
            {
                perror( "Error: Mutex initialisation failed\n");
            }
*/
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
    int numLines = 0;
    /* CPU names for differenciating which thread does which task */
    char* cpu1 = "CPU-1";

    /* THIS WILL PROBABLY HAVE TO CHANGE
     * MAKE THIS OPEN AND APPEND INSTEAD */
    FILE* logFile = fopen( "simulation_log", "w" );

    /* Thread declarations */
    pthread_t cpuThread;
    pthread_t taskThread;

    if( logFile != NULL )
    {
        /* Fill up all the tasks from the file to a linked list */
        numLines = getNumLines( taskFileName );
        tasks = makeList( numLines );

        fileToLL( tasks, taskFileName );

        /* Fill ready queue */
/*        while( !isFull( readyQ ) )
        {
            task( logFile );
        }
*/
        /* Start threads */
        pthread_create( &taskThread, NULL, task, NULL );
        pthread_create( &cpuThread, NULL, cpu, cpu1 );

        /* Join threads */
        pthread_join( taskThread, NULL );
        pthread_join( cpuThread, NULL );

/*
        THIS IS THE NON THREAD IMPLEMENTATION
        while( !isEmpty( readyQ ) )
        {
            cpu( &( readyQ->head->task ) );
            removeFirst( readyQ );

            if( readyQ->size <= readyQ->max - 2 )
            {
                task( logFile );
            }
        }
        fclose( logFile );
*/
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
    int taskID, burstTime, hr, min, sec;
    /* Time related declarations */
    time_t timer;
    char servTime[50];
    char* cpuNum;
    struct tm* serviceTime;
    /* Represents the CPU name */
    cpuNum = ( char* )inCPUNum;

if( isEmpty( readyQ ) )
{
    printf( "Ready queue is empty\n" );
}
else
{
    printf( "Ready queue is not empty\n" );
}

    while( !isEmpty( readyQ ) )
    {
        pthread_mutex_lock( &lock );
/* printf("CPU unlocked\n"); */
/*        while( )
        pthread_cond_wait( &condition, &lock );
*/
        /* Take info from head of tasks linked list (ie. first task in queue) */
        taskID = readyQ->head->task.taskID;
        burstTime = readyQ->head->task.burstTime;
        hr = readyQ->head->task.arrivalTime.hour;
        min = readyQ->head->task.arrivalTime.minute;
        sec = readyQ->head->task.arrivalTime.second;

        /* This represents current time, ie. service time */
        time( &timer );
        serviceTime = localtime( &timer );

        /* Converts to string for ease of use */
        strftime( servTime, 50, "%H:%M:%S", serviceTime );

        /* Sleeps for time period simulating CPU completing task */
        sleep( burstTime * 0.01 );
        printf( "Statistics for %s:\nTask: %d\nArrivalTime: %02d:%02d:%02d\nService Time: %s\n", cpuNum, taskID, hr, min, sec, servTime );

        /* Once task is finished, remove it from queue */
        removeFirst( readyQ );
        numTasks--;

/*        pthread_cond_signal( &condition );
*/
    pthread_mutex_unlock( &lock );
    }
    printf("CPU numTasks: %d\n", numTasks );

    return NULL;
}

/* Used code from https://stackoverflow.com/questions/3673226/how-to-print-time-in-format-2009-08-10-181754-811 to format time */
void* task( void* logFile )
{
    /* Variable declarations */
    int ii;
    char logStr[50];

    /* Time related variables */
    int taskID, burstTime, hr, min, sec;
    time_t timer;
    char currTime[50];
    struct tm* tmInfo;

    /* Takes in the logfile to write to
     * WILL PROBALBLY NEED TO CHANGE THIS */
    logFile = ( FILE* )logFile;

    /* Keep looping and filling in any tasks that have been completed */
    while( !isEmpty( tasks ) )
    {
        pthread_mutex_lock( &lock );
/* printf( "Tasks unlocked\n" );
*/
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
                        /* writeLog( logFile, logStr ); */
                        removeFirst( tasks );
/* printf("Task numTasks: %d\n", numTasks );
*/
                    }
                }
            }
            /* pthread_cond_signal( &condition ); */
        }
        pthread_mutex_unlock( &lock );
    }
    return NULL;
}
