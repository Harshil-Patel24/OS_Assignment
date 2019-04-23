#include "scheduler.h"

int main( int argc, char** argv )
{
    if( argc == 2 )
    {
        int maxSize;

        if( atoi( argv[1] ) != 0 )
        {
            maxSize = atoi( argv[1] );
        }
        else
        {
            perror( "Error: Usage- ./scheduler <ready queue size>" )
        }

        LinkedList readyQ = makeList( maxSize );
    }
    return 0;
}
