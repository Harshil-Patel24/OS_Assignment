#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_time.h"

typedef struct
{
    int taskID;
    int burstTime;
    Time arrivalTime;
}
Task;

/* This is a specific linked list for this program */
typedef struct LinkedListNode
{
    Task task;
    struct LinkedListNode* next;
    struct LinkedListNode* prev;
}
LinkedListNode;

typedef struct
{
    int size;
    int max;
    LinkedListNode* head;
    LinkedListNode* tail;
}
LinkedList;

/* Method declarations */
LinkedList* makeList( int );
void insertLast( LinkedList*, int, int, int, int, int );
void printList( LinkedList* );
void freeList( LinkedList* );
void removeLast( LinkedList* );
void removeFirst( LinkedList* );
int isFull( LinkedList* );
int isEmpty( LinkedList* );

#endif
