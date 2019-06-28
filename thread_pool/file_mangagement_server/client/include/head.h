#ifndef __HEAD_H__
#define __HEAD_H__

#include<stdio.h>
#include<sys/uio.h>
#include<sys/epoll.h>
#include<netdb.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<pthread.h>
#include<malloc.h>
#include<signal.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/wait.h>
#include<sys/select.h>
#include<sys/time.h>
#include<sys/mman.h>
#include<errno.h>
#include<fcntl.h>
#include<grp.h>
#include<pwd.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<dirent.h>

#define FILENAME "file"
#define BUFSIZE 1024 

#define ARGU_CHECK(argu, num){ if(argu != num) \
	{ printf("argument error\n"); return -1;}}

#define ERROR_CHECK(ret, num, err){ if(ret == num) \
	{ perror(err); return -1;}}

//process managed by process of parents
typedef struct{
	pid_t pid;
	int	  fd;
	short busy;
}process_data;

typedef struct{
	int flag;		//1 is cd, 2 is ls, 3 is puts, 4 is gets, 5 is remove, 6 is pwd
	int dataLen;
	char buf[BUFSIZE];
}Train_t;

#endif
