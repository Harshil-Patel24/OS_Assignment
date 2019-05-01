#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "my_time.h"
#include "linked_list.h"
#include "file_io.h"

void schedule( char* );
void* cpu( void* );
void* task( void* );

#endif
