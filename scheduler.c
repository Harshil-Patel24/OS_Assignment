#include "scheduler.h"

LinkedList* readyQ;
LinkedList* tasks;
int numTasks;
int totalWaitingTime;
int totalTurnAroundTime;

pthread_mutex_t lock;

/* scheduler.h will run the program and track most of its flow
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
            if( pthread_mutex_init( &lock, NULL ) == 0 )
            {
                maxSize = atoi( argv[2] );
                readyQ = makeList( maxSize );
                fileName = argv[1];
                schedule( fileName );
            }
            else
            {
                perror( "Error: Mutex initialisation failed\n");
            }
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
    char* cpu1 = "CPU-1";
    /* THIS WILL PROBABLY HAVE TO CHANGE
     * MAKE THIS OPEN AND APPEND INSTEAD */
    FILE* logFile = fopen( "simulation_log", "w" );

    pthread_t cpuThread;
    pthread_t taskThread;

    if( logFile != NULL )
    {
        numLines = getNumLines( taskFileName );
        tasks = makeList( numLines );

        fileToLL( tasks, taskFileName );

        /* Fill ready queue */
/*        while( !isFull( readyQ ) )
        {
            task( logFile );
        }
*/
        pthread_create( &cpuThread, NULL, cpu, cpu1 );
        pthread_create( &taskThread, NULL, task, NULL );

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

void* cpu( void* cpuNum )
{
    int taskID, burstTime;
    cpuNum = ( char* )cpuNum;

    int hr, min, sec;
    time_t timer;
    char servTime[50];
    struct tm* serviceTime;

    while( !isEmpty( readyQ ) )
    {
/* printf("CPU\n"); */
        pthread_mutex_lock( &lock );

        taskID = readyQ->head->task.taskID;
        burstTime = readyQ->head->task.burstTime;
        hr = readyQ->head->task.arrivalTime.hour;
        min = readyQ->head->task.arrivalTime.minute;
        sec = readyQ->head->task.arrivalTime.second;


        time( &timer );
        serviceTime = localtime( &timer );

        strftime( servTime, 50, "%H:%M:%S", serviceTime );

        sleep( burstTime * 0.01 );
        printf( "Statistics for %s:\nTask: %d\nArrivalTime: %02d:%02d:%02d\nService Time: %s\n", taskID, hr, min, sec, servTime );

        removeFirst( readyQ );
        numTasks--;
        pthread_mutex_unlock( &lock );
printf("CPU numTasks: %d", numTasks );
    }

    return NULL;
}

/* Used code from https://stackoverflow.com/questions/3673226/how-to-print-time-in-format-2009-08-10-181754-811 to format time */
void* task( void* logFile )
{
    int ii;
    char logStr[50];
    
    int taskID, burstTime, hr, min, sec;
    time_t timer;
    char currTime[50];
    struct tm* tmInfo;

    logFile = ( FILE* )logFile;


    while( !isEmpty( tasks ) )
    {
        pthread_mutex_lock( &lock );
        if( readyQ->size <= readyQ->max - 2 )
        {
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

/*                        strftime( currTime, 50, "%H:%M:%S", tmInfo );
*/
                        sprintf( logStr, "================================\n%d: %d\nArrival Time: %s\n", taskID, burstTime, currTime );
                        numTasks++;
                        /* writeLog( logFile, logStr ); */
                        removeFirst( tasks );
printf("Task numTasks: %d", numTasks );

                    }
                }
            }
        }
        pthread_mutex_unlock( &lock );
    }


    return NULL;
}
