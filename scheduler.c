#include "scheduler.h"

/* scheduler.h will run the program and track most of its flow
 * it will have cpu() and task() methods */
int main( int argc, char** argv )
{
    /* Checks if correct number of arguments */
    if( argc == 3 )
    {
        /* Variable declarations */
        int maxSize = 0;
        LinkedList* readyQ = NULL;
        char* fileName = NULL;

        /* Third argument should be maximum queue sized required */
        if( atoi( argv[2] ) > 0 )
        {
            maxSize = atoi( argv[2] );
            readyQ = makeList( maxSize );
            fileName = argv[1];
            schedule( readyQ, fileName );
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

void schedule( LinkedList* readyQ, char* taskFileName )
{
    LinkedList* tasks = NULL;
    int numLines = 0;

    numLines = getNumLines( taskFileName );
    tasks = makeList( numLines );

    fileToLL( tasks, taskFileName );

    /* printList( tasks ); */

    /* Fill ready queue */
    while( !isFull( readyQ ) )
    {
        task( tasks, readyQ );
    }

    /* printList( readyQ ); */

    while( !isEmpty( readyQ ) )
    {
        cpu( readyQ->head->taskID, readyQ->head->burstTime );
        removeFirst( readyQ );

        if( readyQ->size <= readyQ->max -2 )
        {
            printf( "More added\n" );
            task( tasks, readyQ );
        }
    }

}

void cpu( int taskID, int burstTime )
{
    sleep( burstTime * 0.01 );
    printf( "Task ID: %d\nBurst Time: %d\n", taskID, burstTime );
}

void task( LinkedList* tasks, LinkedList* readyQ )
{
    int ii;
    for( ii = 0; ii < 2; ii++ )
    {
        if( !isFull( readyQ ) )
        {
            insertLast( readyQ, tasks->head->taskID, tasks->head->burstTime );
            removeFirst( tasks );
        }
    }
}
