#ifndef __POOL_MANAGEMENT_H__
#define __POOL_MANAGEMENT_H__

#include "head.h"
#include "pthread_and_queue.h"


typedef struct {
	pthread_t *pthid;			//id of all thread
	pthread_cond_t cond;		//inform thread to job when pthread_cond_signal called
	thread_queue queue;			//thread queue
	int Flag;					//start flag
	int threadNum;				//number of thread working
}pool_management;

int pool_init(pool_management *, int, int);

int pool_start(pool_management *);

void *thread_fun(void *);

#endif
