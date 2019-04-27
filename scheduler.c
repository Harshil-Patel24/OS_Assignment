#include "scheduler.h"

LinkedList* readyQ;
LinkedList* tasks;

pthread_mutex_t lock;

/* scheduler.h will run the program and track most of its flow
 * it will have cpu() and task() methods */
int main( int argc, char** argv )
{
    /* Checks if correct number of arguments */
    if( argc == 3 )
    {
        /* Variable declarations */
        int maxSize = 0;
        char* fileName = NULL;


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
        pthread_create( &cpuThread, NULL, cpu, NULL );
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

void* cpu( void* nothingLMAO )
{
    int taskID, burstTime;
printf("CPU\n");
    while( !isEmpty( readyQ ) )
    {
printf("CPU\n");
    /*    pthread_mutex_lock( &lock ); */

        taskID = readyQ->head->task.taskID;
        burstTime = readyQ->head->task.burstTime;

        sleep( burstTime * 0.01 );
        printf( "Task ID: %d\nBurst Time: %d\n", taskID, burstTime );

        removeFirst( readyQ );

    /*    pthread_mutex_unlock( &lock ); */
    }

    return NULL;
}

/* Used code from https://stackoverflow.com/questions/3673226/how-to-print-time-in-format-2009-08-10-181754-811 to format time */
void* task( void* logFile )
{
    int ii;
    char logStr[50];
    int taskID, burstTime;
    time_t timer;
    char currTime[50];
    struct tm* tmInfo;

    logFile = ( FILE* )logFile;


    while( !isEmpty( tasks ) )
    {
/* printf( "Task\n" ); */
        /* pthread_mutex_lock( &lock ); */
        if( readyQ->size <= readyQ->max - 2 )
        {
/* printf( "size <= max\n" ); */
            for( ii = 0; ii < 2; ii++ )
            {
                if( !isFull( readyQ ) )
                {
/* printf( "ready queue is not full\n" ); */
                    if( !isEmpty( tasks ) )
                    {
/* printf( "tasks queue is not empty\n" ); */
                        time( &timer );
                        tmInfo = localtime( &timer );

                        strftime( currTime, 50, "%H:%M:%S", tmInfo );

                        taskID = tasks->head->task.taskID;
                        burstTime = tasks->head->task.burstTime;
                        insertLast( readyQ, taskID, burstTime );
                        sprintf( logStr, "================================\n%d: %d\nArrival Time: %s\n", taskID, burstTime, currTime );
                        /* writeLog( logFile, logStr ); */
                        removeFirst( tasks );
                    }
                }
            }
        }
        /* pthread_mutex_unlock( &lock ); */
    }


    return NULL;
}
