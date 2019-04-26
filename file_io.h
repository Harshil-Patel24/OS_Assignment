#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdio.h>
#include <stdlib.h>
#include  "linked_list.h"

int getNumLines( char* );
void fileToLL( LinkedList*, char* );
void writeLog( FILE*, char* );

#endif
