#ifndef __PTHREAD_AND_QUEUE_H___
#define __PTHREAD_AND_QUEUE_H___

#include "head.h"

typedef struct node{
	int childFd;			//file description of client socket
	struct node *pNext;		//point to next thread node
}Node_thread;

typedef struct{
	Node_thread *qHead, *qTail;		//head and tail of queue
	pthread_mutex_t qMutex;			//mutex of thread
	int qSize;						//size of queue
	int qCapacity;					//capacity of queue
}thread_queue;

int queue_init(thread_queue *, int);

int queue_input(thread_queue *, Node_thread *);

int queue_output(thread_queue *, Node_thread **);

#endif
