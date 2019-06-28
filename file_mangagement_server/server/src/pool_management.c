#include "../include/pool_management.h"
#include "../include/deal_sql.h"
#include "../include/analyse_layer.h"


void *thread_fun(void *p){
	pool_management *pool = (pool_management *)p;
	thread_queue *queue = &pool->queue;
	Node_thread *pNode;
	int ret;
	while(1){
		pthread_mutex_lock(&queue->qMutex);
		if(queue->qSize == 0){
			pthread_cond_wait(&pool->cond, &queue->qMutex);
			//printf("I'm threads, and we is OK\n");
		}
		ret = queue_output(queue, &pNode);
		pthread_mutex_unlock(&queue->qMutex);

		//deal require of client
		if(ret == 0){		
			//read(pNode->childFd, buf, 30);
			//printf("read have problem\n");
			//printf("%s\n", buf);

			//we deal require of client when get file description of client
			ret = sql_log(pNode);
			analyse_order(pNode);
			free(pNode);
		}
	}
	pthread_exit(NULL);
}



int pool_init(pool_management *pool, int threadNum, int capacity){
	memset(pool, 0, sizeof(pool_management));
	pool->pthid = (pthread_t *)calloc(threadNum, sizeof(pthread_t));
	if(pool->pthid == NULL){
		perror("calloc");
		return -1;
	}
	pool->threadNum = threadNum;
	int ret;
	ret = pthread_cond_init(&pool->cond, NULL);
	if(ret != 0){
		perror("pthread_cond_init");
		return -1;
	}
	queue_init(&pool->queue, capacity);
	return 0;
}


int pool_start(pool_management *pool){
	int i; 
	if(pool->Flag == 0){
		for(i = 0; i < pool->threadNum; ++i){
			pthread_create(pool->pthid + i, NULL, thread_fun, pool);
		}
	}
	pool->Flag = 1;
	return 0;
}
