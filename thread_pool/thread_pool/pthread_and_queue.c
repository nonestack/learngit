#include "../include/pthread_and_queue.h"


int queue_init(thread_queue *queue, int capacity){
	memset(queue, 0, sizeof(thread_queue));
	pthread_mutex_init(&queue->qMutex, NULL);
	queue->qCapacity = capacity;
	return 0;
}

int queue_input(thread_queue *queue, Node_thread *node){
	if(queue->qTail == NULL){
		queue->qHead = node;
		queue->qTail = node;
	}
	else{
		queue->qTail->pNext = node;
		queue->qTail = queue->qTail->pNext;
	}
	++queue->qSize;
	return 0;
}

int queue_output(thread_queue *queue, Node_thread **ppNode){
	if(queue->qSize == 0){
		//printf("queue is already empty\n");
		return -1;
	}
	*ppNode = queue->qHead;
	queue->qHead = queue->qHead->pNext;
	if(queue->qHead == NULL){
		queue->qTail = NULL;
	}
	--queue->qSize;
	return 0;
}
