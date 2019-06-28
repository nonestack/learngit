#include "../include/head.h"
#include "../include/analyse_layer.h"
#include "../include/pool_management.h"
#include "../include/deal_description.h"
#include "../include/pthread_and_queue.h"

int main(int argc, char *argv[]){
	int ret;
	ARGU_CHECK(argc, 2);
	char ip[20] = {0};
	char port[20] = {0};
	int threadNum, capacity;
	extract_conf(argv[1], ip, port, &threadNum, &capacity);
	printf("ip = %s, port = %s, threadNum = %d, capacity = %d\n", ip, port, threadNum, capacity);
	pool_management pool;
	pool_init(&pool, threadNum, capacity);
	pool_start(&pool);
	
	int addrlen = sizeof(struct sockaddr_in);
	int socketFd;
	int childAddr;
	tcp_init(&socketFd, ip, port);
	//printf("socketFd = %d\n", socketFd);

	int clientFd;
	thread_queue *queue = &pool.queue;
	Node_thread *pNode;

	
	while(1){
		pNode = (Node_thread *)calloc(1, sizeof(Node_thread));
		clientFd = accept(socketFd, (struct sockaddr *)&pNode->addrInfo, &addrlen);
		pNode->childFd = clientFd;
		strcat(pNode->cpath, "/");
		//write Log
		int fd;
		time_t now;
		char now_time[BUFSIZE] = {0};
		now = time(NULL);
		fd = open(LOG_NAME, O_CREAT | O_RDWR, 0664);
		ERROR_CHECK(fd, -1, "open");
		ret = lseek(fd, 0, SEEK_END);
		ERROR_CHECK(ret, -1, "lseek");
		write(fd, inet_ntoa(pNode->addrInfo.sin_addr), strlen(inet_ntoa(pNode->addrInfo.sin_addr)));
		write(fd, ": Log in ", 9);
		strcpy(now_time, ctime(&now));
		write(fd, now_time, strlen(now_time));

		write(fd, "\n", 1);
		close(fd);
		//allocate thread
		pthread_mutex_lock(&queue->qMutex);
		queue_input(queue, pNode);
		pthread_mutex_unlock(&queue->qMutex);
		pthread_cond_signal(&pool.cond);
		//printf("I'm normal\n");
	}


	return 0;
}
