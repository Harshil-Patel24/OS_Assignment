#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "file_io.h"
#include "linked_list.h"

void schedule( LinkedList*, char* );
void* cpu( void* );
void task( LinkedList*, LinkedList*, FILE* );

#endif
