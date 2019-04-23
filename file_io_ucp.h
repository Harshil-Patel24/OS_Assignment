#ifndef _LINKEDLISTH_
#define _LINKEDLISTH_

#include "linkedList.h"

#endif

/* Reads the data from selected file into linked list */
void readFile( char*, LinkedList*, LinkedList* );
/* Counts number of words in imported string (to check if file is valid) */
int numWords( char* );
/* Writes the imported string to the imported file
 * used to write to logfile */
void writeLog( FILE*, char* );
