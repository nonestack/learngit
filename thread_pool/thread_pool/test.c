/***************************************************************************************************
举个栗子：
	整个线程池就像一个饭店
	线程就是一个厨师
	队列的容量就像整个饭店厨师的数量
	队列的大小就像现在在饭店厨师的数量

	假如一个饭店有10个厨师
	但在空闲的时候所有厨师都在厨房会太浪费资源，厨房是做菜的地方，里面的就是工作的线程，
	不对，是工作的厨师
	(当然你觉得我了花钱雇的人就得呆在这，要不然就亏了，呃.....这个就过分了)
	如果在空闲的时候就0-1一个厨师在厨房的时候
	顾客突然多的时候要把厨师从家叫过来会耽误顾客的时间(customer is god, 应该是这样的)
	上面说的就是多线程，当需要线程的时候才会创造线程，会耽误时间
	线程池就是给厨师们弄个休闲场所，当客人来的时候就把厨师从休闲场所揪出来。(嗯，应该是这样的)
	揪厨师的人就是你，哦，不对，是管理队列的结构体
	这样就会有下面的三个结构体
****************************************************************************************************/

//线程结构体(厨师)
typedef struct node{
	int childFd;			//file description of client socket
	//日常指向下个结点
	struct node *pNext;		//point to next thread node
}Node_thread;

//存放线程的队列(娱乐场所)
typedef struct{
	
	Node_thread *qHead, *qTail;		//head and tail of queue
	//因为队列是共享资源，所以要加锁
	pthread_mutex_t qMutex;			//mutex of thread
	int qSize;						//size of queue
	int qCapacity;					//capacity of queue
}thread_queue;

//管理线程队列的管理者结构体(剥削者，对就是你)
typedef struct {
	pthread_t *pthid;			//id of all thread
	//cond是通知的等待的线程(就是你揪人)
	pthread_cond_t cond;		//inform thread to job when pthread_cond_signal called
	thread_queue queue;			//thread queue
	int Flag;					//start flag
	int threadNum;				//number of thread working
}pool_management;

//那些函数应该可以看懂吧，不懂跟我说

