//***********************************************************
/*
  File: 	queue.c
  Description:	Implements a basic queue of size Q_SIZE
  methods:	1) init () initialize before using the queue
		2) print_q () displays valid contents of the queue
		3) enqueue () inserts an element into queue
 		4) dequeue () removes an element from queue
		5) get_count () gets number of elements in queue
*/
//***********************************************************

#ifndef QUEUE_H
#define QUEUE_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<string.h>
#include"user_appli.h"
#define Q_SIZE 16 

typedef unsigned int u32;


typedef struct 
{
  data_t queue[Q_SIZE];
  data_t *head,*tail;
  int count;
}queue_t;

 int get_count(queue_t *);
 int enqueue(queue_t *, data_t);
 int dequeue(queue_t *, data_t *);
 void init(queue_t *);

#endif
