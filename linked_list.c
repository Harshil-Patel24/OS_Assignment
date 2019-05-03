#include "linked_list.h"

#define MAXSIZE 500

/**
 * METHOD: makeList
 * PURPOSE: To make a linked list, initilise values and return the list
 *          Used to make instanciating linked lists easier and reduces
 *          potential error (ie. forgetting to initialise head, tail etc. )
 **/
LinkedList* makeList( int maxSize )
{
    /* Declare the LinkedList */
    LinkedList* list = NULL;

    /* Allocates heap memory for the list */
    list = ( LinkedList* )malloc( sizeof( LinkedList ) );

    /* Initialises the values */
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    /* Ensures mazsize is a positive number */
    if( maxSize > 0 )
    {
        list->max = maxSize;
    }
    /* Otherwise set the default of 10 and notify user */
    else
    {
        list->max = 10;
        /* Sends error message to stderr and also to screen that default size was used */
        perror( "Error: Invalid max linked list size\n default max size used of 10" );
        printf( "Invalid max linked list size, default max size used of 10\n" );
    }

    return list;
}

/**
 * METHOD: insertLast
 * PURPOSE: To create a node from imported data and insert it at the end of imported list
 *          This saves me having to create a list node first and then add it at the end
 *          by manually chaging next and prev pointers
 **/
void insertLast( LinkedList* list, int taskID, int burstTime, int hr, int min, int sec )
{
    if( list->size < list->max )
    {
        /* Allocates heap memory for the node */
        LinkedListNode* newNd = ( LinkedListNode* )malloc( sizeof( LinkedListNode ) );
        /* Data will also be stored on heap */
/*        newNd.taskID = ( int* )malloc( sizeof( int ) ); */

        /* Initialises data within the list node */
        newNd->prev = NULL;
        newNd->next = NULL;

        newNd->task.taskID = taskID;
        newNd->task.burstTime = burstTime;
        newNd->task.arrivalTime.hour = hr;
        newNd->task.arrivalTime.minute = min;
        newNd->task.arrivalTime.second = sec;

        /* Inserts the node at the end of the list if there are already nodes in the list */
        if( list->head != NULL )
        {
            newNd->prev = list->tail;

            list->tail = newNd;
            newNd->prev->next = newNd;

            list->size++;
        }
        /* If there is no node, then make this new node the head */
        else
        {
            list->head = newNd;
            list->tail = newNd;

            list->size++;
        }
    }
    /* Otherwise print error to stderr */
    else
    {
        perror( "Error: Linked list is full\n" );
    }
}

/**
 * METHOD: printList
 * PURPOSE: To print out the contents of the imported list
 **/
void printList( LinkedList* list )
{
    /* Initialise variables */
    LinkedListNode* curr = NULL;
    int ii = 0;

    /* Iterates through the list and prints out each element */
    curr = list->head;
    for( ii = 0; ii < list->size; ii++ )
    {
        printf( "Task ID: %d\nBurst Time: %d\n", curr->task.taskID, curr->task.burstTime );
        curr = curr->next;
    }
}

/**
 * METHOD: freeList
 * PURPOSE: To free all of the nodes in the imported list and finally the list itself
 **/
void freeList( LinkedList* list )
{
    /* Initialise variables */
    LinkedListNode* curr = NULL;
    LinkedListNode* temp = NULL;

    /* Loops though to free all nodes in the list starting from the tail
     * and moving backwards */
    if( list != NULL )
    {
        curr = list->tail;
        while( curr != NULL )
        {
            temp = curr;
            curr = curr->prev;

            /* Free's the node */
            free( temp );
            temp = NULL;
        }
        /* Finally free's the entire list */
        free( list );
        list = NULL;
    }
    else
    {
        printf( "List does not exist\n" );
    }
}

/**
 * METHOD: removeLast
 * PURPOSE: To remove the last element in the imported list and free it
 *          This method is not used in my program, it is a method I intended
 *          to use but decided to free all at once, I have kept the method
 *          incase I decided to use it later
 **/
void removeLast( LinkedList* list )
{
    /* Initailise variables */
    LinkedListNode* last = NULL;
    LinkedListNode* secLast = NULL;

    /* Disconnects the tail from the list */
    last = list->tail;
    secLast = last->prev;
    list->tail = secLast;

    /* Decrement size of linked list */
    list->size--;

    /* Stops it from pointing anywhere important */
    last->next = NULL;
    last->prev = NULL;

    /* Free the last node */
    free( last );
    last = NULL;
}

/**
 * METHOD: removeFirst
 * PURPOSE: To remove the first element in the imported list and free it
 *          This is used as the ready queue is a FIFO queue
 ***/
void removeFirst( LinkedList* list )
{
    /* Initailise variables */
    LinkedListNode* first = NULL;
    LinkedListNode* second = NULL;

    /* Disconnects the head from the list */
    first = list->head;
    second = first->next;
    list->head = second;

    /* Decrement size of linked list */
    list->size--;

    /* Stops it from pointing anywhere important */
    first->next = NULL;
    first->prev = NULL;

    /* Free the first node */
    free( first );
    first = NULL;


/*
    LinkedListNode* tempNode;

    if( list->head == NULL )
    {
    }
    else if( list->head->next == NULL )
    {
        free( list->head );
        list->head = NULL;
        list->tail = NULL;
    }
    else
    {
        tempNode = list->head->next;
        free( list->head );
        list->head = NULL;
        list->head = tempNode;
        list->head->prev = NULL;
    }
*/
}

/* Will return 1 if full, 0 if not full */
int isFull( LinkedList* ll )
{
    int full = 1;

    if( ll->size < ll->max )
    {
        full = 0;
    }

    return full;
}

int isEmpty( LinkedList* ll )
{
    int empty = 1;

    if( ll->size > 0 )
    {
        empty = 0;
    }
/*
    int empty = 1;

    if( ll->head != NULL )
    {
        empty = 0;
    }
*/
    return empty;
}
