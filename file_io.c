#include "file_io.h"

int numLines( char* fileName )
{
    FILE* fp;
    int numLines;
    char c;

    c = '';
    fp = fopen( fileName );

    if( fp != NULL )
    {
        numLines = 0;

        while( !eof( c ) )
        {
            c = fgetc( fp );

            if( c == '\n' )
            {
                numLines++;
            }
        }
    }
    else
    {
        perror( "Error: Couldn't read from file\n" );
    }

    return numLines;
}
