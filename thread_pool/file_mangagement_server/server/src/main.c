#include "../include/head.h"
#include "../include/deal_description.h"
#include "../include/pool_management.h"
#include "../include/pthread_and_queue.h"

int main(int argc, char *argv[]){
	ARGU_CHECK(argc, 2);
	char ip[20] = {0};
	char port[20] = {0};
	int threadNum, capacity;
	extract_conf(argv[1], ip, port, &threadNum, &capacity);
	printf("ip = %s, port = %s, threadNum = %d, capacity = %d\n", ip, port, threadNum, capacity);
	pool_management pool;
	pool_init(&pool, threadNum, capacity);
	pool_start(&pool);
	
	int socketFd;
	int childAddr;
	tcp_init(&socketFd, ip, port);
	//printf("socketFd = %d\n", socketFd);

	int clientFd;
	thread_queue *queue = &pool.queue;
	Node_thread *pNode;
	while(1){
		clientFd = accept(socketFd, NULL, NULL);
		pNode = (Node_thread *)calloc(1, sizeof(Node_thread));
		pNode->childFd = clientFd;
		pthread_mutex_lock(&queue->qMutex);
		queue_input(queue, pNode);
		pthread_mutex_unlock(&queue->qMutex);
		pthread_cond_signal(&pool.cond);
		printf("I'm normal\n");
	}


	return 0;
}
