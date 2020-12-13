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

#include"queue.h"
#include"user_appli.h"
//***********************************************************
void init(queue_t *que)
{
 memset((void *)(que->queue), 0, Q_SIZE*sizeof(data_t));
 que->head = que->queue;
 que->tail = que->queue;
 que->count = 0;
}

//***********************************************************
int get_count(queue_t *que)
{
  return que->count;
}

//***********************************************************
int enqueue(queue_t *que,  data_t a)
{
  if(get_count(que) == Q_SIZE)
   {
     return -1;
   }
   *(que->head) = a;
   que->count++;

  if((que->head +1) == (que->queue + Q_SIZE))
  {
    que->head = que->queue;
  }
  else
  { 
    que->head++;
  }

  return 1;
}

//***********************************************************
int dequeue(queue_t *que, data_t *a)
{
  if (get_count(que) == 0)
  {
    return -1;
  }
  
  *a = *(que->tail);
   que->count--;
  if((que->tail + 1) == (que->queue + Q_SIZE))
  {
    que->tail = que->queue;
  }
  else
  { 
    que->tail++;
  }
 return 1;
}
//***********************************************************



