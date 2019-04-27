#include "scheduler.h"

LinkedList* readyQ;
LinkedList* tasks;

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
            maxSize = atoi( argv[2] );
            readyQ = makeList( maxSize );
            fileName = argv[1];
            schedule( fileName );
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
    FILE* logFile = fopen( "simulation_log", "w" );

/*    pthread_t cpuThread; */
/*    pthread_create( cpuThread, NULL, cpu, &( readyQ->head->task ) ); */


    if( logFile != NULL )
    {
        numLines = getNumLines( taskFileName );
        tasks = makeList( numLines );

        fileToLL( tasks, taskFileName );

        /* printList( tasks ); */

        /* Fill ready queue */
        while( !isFull( readyQ ) )
        {
            task( logFile );
        }

        /* printList( readyQ ); */

        while( !isEmpty( readyQ ) )
        {
            cpu( &( readyQ->head->task ) );
            removeFirst( readyQ );

            if( readyQ->size <= readyQ->max - 2 )
            {
                /* printf( "More added\n" ); */
                task( logFile );
            }
        }
        fclose( logFile );
    }
    else
    {
        perror( "Error: Couldn't open logfile to write\n" );
    }

}

void* cpu( void* nothingLMAO )
{
    int taskID, burstTime;

    taskID = readyQ->head->task.taskID;
    burstTime = readyQ->head->task.burstTime;

    sleep( burstTime * 0.01 );
    printf( "Task ID: %d\nBurst Time: %d\n", taskID, burstTime );

    return NULL;
}

/* Used code from https://stackoverflow.com/questions/3673226/how-to-print-time-in-format-2009-08-10-181754-811 to format time */
void task( FILE* logFile )
{
    int ii;
    char logStr[50];
    int taskID, burstTime;
    time_t timer;
    char currTime[50];
    struct tm* tmInfo;

    for( ii = 0; ii < 2; ii++ )
    {
        if( !isFull( readyQ ) )
        {
            if( !isEmpty( tasks ) )
            {
                time( &timer );
                tmInfo = localtime( &timer );

                strftime( currTime, 50, "%H:%M:%S", tmInfo );

                taskID = tasks->head->task.taskID;
                burstTime = tasks->head->task.burstTime;
                insertLast( readyQ, taskID, burstTime );
                sprintf( logStr, "================================\n%d: %d\nArrival Time: %s\n", taskID, burstTime, currTime );
                writeLog( logFile, logStr );
                removeFirst( tasks );
            }
        }
    }
}
