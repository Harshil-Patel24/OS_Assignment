#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdlib.h>
#include <unistd.h>
#include "linked_list.h"
#include "file_io.h"

void schedule( LinkedList*, char* );
void cpu( int, int );
void task( LinkedList*, LinkedList* );

#endif
