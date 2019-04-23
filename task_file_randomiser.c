#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "task_file_randomiser.h"

int main( int argc, char** argv )
{
    if( argc == 1 )
    {
        taskFileCreate( 100 );
    }
    else if( argc == 2 )
    {
        if( atoi( argv[1] ) > 0 )
        {
            taskFileCreate( atoi( argv[1] ) );
        }
        else
        {
            perror( "Error: Invalid parameter, please use this format to run program- ./task_file_randomiser <number-of-tasks>\n" );
        }
    }
    else
    {   
        perror( "Error: Invalid number of parameters, please use this format to run program- ./task_file_randomiser <number-of-tasks>\n" );   
    }
    
    return 0;
}

void taskFileCreate( int numTasks )
{
    FILE *fp;
    int ii;
    int taskTime;
    int success;

    srand( time( NULL ) );    


    fp = fopen( "task_file", "a" );

    if( fp == NULL )
    {
        perror( "Error: Couldn't open file- task_file to append\n" );
    }
    else
    {
        for( ii = 1; ii <= numTasks; ii++ )
        {

            taskTime = rand() % 50;
            success = fprintf( fp, "%d %d\n", ii, taskTime );  
            if( success < 0 )   
            {
                perror( "Error: Couldn't append to file\n" );
            }           
        }
    }
       
    fclose( fp );
}   
