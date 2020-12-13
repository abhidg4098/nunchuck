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
void print_q(queue_t *que)
{
}

//***********************************************************
int enqueue(queue_t *que,  data_t a)
{
  if(get_count(que) == Q_SIZE)
   {
     printf("enqueue>> queue full \n");
     return -1;
   }
   *(que->head) = a;
   printf("enqueue>> stored at loc head = %u and x_axis = %d \n", que->head, a.x_axis);
  que->count++;
  printf("count = %d , %d \n", que->count, get_count(que));

  if((que->head +1) == (que->queue + Q_SIZE))
  {
    printf("retrack %d \n ", que->count);
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
    printf("dequeue>> Queue empty! \n");
    return -1;
  }
  
  *a = *(que->tail);
   que->count--;
   //printf("dequeue>> extracted token %d, more %d remains \n", a->token, que->count);
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

/*

//test code


int main()
{
 element_t sample;
 queue_t *temp_que = malloc(sizeof(queue_t));
 int time1, tok1, iterations;
 printf("enter the data \n");
 printf("success \n");

 init(temp_que);
 iterations = 5;
while (iterations > 0)
{
 iterations--;

 scanf("%d",&time1);
 scanf("%d",&tok1);
  printf("iterations left: %d \n", iterations);

 sample.time = (u32)time1;
 sample.token = tok1;

 if(1 == enqueue(temp_que, sample))
  printf("success \n");
 else
  printf("issue \n");
 print_q(temp_que);

}

 while (iterations < Q_SIZE)
 {
  iterations++;
  if(1 == dequeue(temp_que, &sample))
  {
    printf("success \n");
    printf("Extracted data: %d, %d", sample.time, sample.token);
  }
 else
  printf("issue \n");

  print_q(temp_que);

 }
 free(temp_que);
 return 0;
}

*/
