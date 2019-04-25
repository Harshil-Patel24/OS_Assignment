#include "file_io.h"
#define BUFFER 50

int getNumLines( char* fileName )
{
    FILE* fp;
    int numLines;
    char c;

    fp = fopen( fileName, "r" );

    if( fp != NULL )
    {
        numLines = 0;

        while( c != EOF )
        {
            c = fgetc( fp );

            if( c == '\n' )
            {
                numLines++;
            }
        }
        fclose( fp );
    }
    else
    {
        perror( "Error: Couldn't read from file\n" );
    }
    return numLines;
}

void fileToLL( LinkedList* ll, char* fileName )
{
    FILE* fp;
    char* fileString;
    int taskNo, burstTime;

    fp = fopen( fileName, "r" );

    if( fp != NULL )
    {
        fgets( fileString, BUFFER, fp );
        scanf( fileString, "%d %d", &taskNo, &burstTime );

        insertLast( ll, taskNo, burstTime );
    }
    else
    {
        perror( "Error: File format is wrong\n" );
    }
}
