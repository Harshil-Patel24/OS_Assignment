#include "file_io.h"
#define BUFFER 50

int getNumLines( char* fileName )
{
    FILE* fp = NULL;
    int numLines = 0;
    char c = 0;

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
printf( "Number of lines (getNumLines()): %d\n", numLines );
    return numLines;
}

void fileToLL( LinkedList* ll, char* fileName )
{
    FILE* fp = NULL;
    char fileString[50];
    int taskNo = 0;
    int burstTime = 0;

    fp = fopen( fileName, "r" );

    if( fp != NULL )
    {
        while( fgets( fileString, BUFFER, fp ) != NULL )
        {
            sscanf( fileString, "%d %d", &taskNo, &burstTime );
            insertLast( ll, taskNo, burstTime, 0, 0, 0 );
        }
        fclose( fp );
    }
    else
    {
        perror( "Error: File could not be opened\n" );
    }
}

/**
 * METHOD: writeLog
 * PURPOSE: To print imported string into the imported file,
 * for our purposes that will be to the logfile
 * REFERENCE: Taken from my (Harshil Patel- 19460606) UCP assignment
 **/
void writeLog( FILE* logFile, char* logStr )
{
    fprintf( logFile, "%s", logStr );
}
