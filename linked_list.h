#include <stdio.h>

/* This is a specific linked list for this program */
typedef struct LinkedListNode
{
    char* data;
    struct LinkedListNode* next;
    struct LinkedListNode* prev;
} 
LinkedListNode;

typedef struct
{
    int size;
    int max;
    LinkedListNode* head;
    LinkedListNode* tail
} 
LinkedList;

/* Method declarations */
LinkedList* makeList( void );
void insertLast( LinkedList*, char* );
void printList( LinkedList* );
void freeList( LinkedList* );
void removeLast( LinkedList* );
