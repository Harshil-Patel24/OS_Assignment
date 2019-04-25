#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdlib.h>
#include "linked_list.h"
#include "file_io.h"

void schedule( LinkedList*, char* )
void cpu( void );
void task( LinkedList* );

#endif
