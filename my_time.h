#ifndef MY_TIME_H
#define MY_TIME_H

#include <stdio.h>

typedef struct
{
    int hour;
    int minute;
    int second;
}
Time;

int timeToSec( Time );

#endif
