#include "scheduler.h"

/* scheduler.h will run the program and track most of its flow
 * it will have cpu() and task() methods */
int main( int argc, char** argv )
{
    /* Checks if correct number of arguments */
    if( argc == 3 )
    {
        /* Variable declarations */
        int maxSize;
        LinkedList* readyQ;

        /* Third argument should be maximum queue sized required */
        if( atoi( argv[2] ) > 0 )
        {
            maxSize = atoi( argv[3] )
            readyQ = makeList( maxSize );
            schedule( readyQ, argv[2] );
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
    LinkedList* tasks;
    int numLines;

    numLines = getNumLines( taskFileName );
    tasks = makeList( numLines );
}

void cpu( void )
{
}
