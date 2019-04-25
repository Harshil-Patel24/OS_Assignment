#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "io.h"

#ifndef _LINKEDLISTH_
#define _LINKEDLISTH_

#include "linkedList.h"

#endif

#define MAXSIZE 500
 
/**
 * METHOD: readFile
 * PURPOSE: Reads the file and scans each entry into a linked list
 *          specifically designed for this purpose.
 *          This method will also perform error checking for file formatting
 **/
void readFile( char* fName, LinkedList* list, LinkedList* errors )
{
    /* Initialise all variables */
    FILE* f = NULL;
    char* data = NULL;
    /* Message for the error LinkedList */
    char errMsg[MAXSIZE];

    f = fopen( fName, "r" );

    /* Checks if the file opening was successful*/
    if( f == NULL )
    {
        sprintf( errMsg, "File could not be opened" );
        insertLast( errors, errMsg );
    }
    else
    {
        /* Allocates heap memory to store the data read from the file */
        data = ( char* )malloc( sizeof( char ) * MAXSIZE );

        /* Reads the entire file line by line */
        while( fgets( data, MAXSIZE, f ) != NULL )
        {
            /* only inserts into list if there are two "words" in data */
            if( numWords( data ) == 2 )
            {
                /* Passes the data into createNode to make the list node */
                insertLast( list, data );
            }
            else
            {
                /* Inserts the error into error list */
                sprintf( errMsg, "Data in file is not in a valid format: %s", data );
                insertLast( errors, errMsg );
            }
        }
        /* Deallocate the data */
        free( data );
        data = NULL;
    }
    /* Only closes file if it was opened */
    if( f != NULL )
    {
        fflush( f );
        fclose( f );
    }
}

/**
 * METHOD: numWords
 * PURPOSE: To calculate the number of word in a string,
 *          used to validate the number of words in the file
 **/
int numWords( char* str )
{
    /* Initialise variables */
    int wordCount = 0;
    int ii = 0;

    /* Word count only gets counted if the string is not empty or doesn't start with a new line */
    if( *str != '\0' || *str != '\n' )
    {
        wordCount = 1;
        /* counts until null terminator */
        for( ii = 0; str[ii] != '\0'; ii++ )
        {
            /* counts words by number of spaces */
            /* the second conditon is to account for a space after the last word */
            if( str[ii] == ' ' && str[ii + 1] != '\n'  )
            {
                wordCount++;
            }
        }
    }
    return wordCount;
}

/**
 * METHOD: writeLog
 * PURPOSE: To print imported string into the imported file,
 * for our purposes that will be to the logfile
 **/
void writeLog( FILE* logFile, char* logStr )
{
    fprintf( logFile, "%s", logStr );
}
