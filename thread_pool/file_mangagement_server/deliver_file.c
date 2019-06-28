#include "deliver_file.h"

int send_file(int socketFd, char *filename){
	Train_t train;
	memset(&train, 0, sizeof(Train_t));
	int fd;
	int ret;
	fd = open(filename, O_RDWR);
	ERROR_CHECK(fd, -1, "open");
	while(ret = read(fd, train.buf, BUFSIZE)){
		write(socketFd,&train, ret + 8);
		memset(train.buf, 0, BUFSIZE);
	}
	train.flag = -1;
	write(socketFd, &train, 4);
	return 0;
}

int recv_file(int socketFd, char *filename){
	int fd;
	Train_t train;
	memset(&train, 0, sizeof(Train_t));
	fd = open(filename, O_CREAT | O_RDWR, 0664);
	ERROR_CHECK(fd, -1, "open");
	int ret;
	while(ret = read(socketFd, &train, sizeof(Train_t))){
		if(train.flag == -1){
			break;
		}
		write(fd, train.buf, ret - 8);
		memset(train.buf, 0, BUFSIZE);
	}
	close(fd);
	return 0;
}

//int recv_cycle(int socketFd, void *p, int dataLen){
//	char *data = (char *)p;
//	
//	int total = 0;
//	int ret;
//	while(total < dataLen){
//		ret = read(socketFd, data + total, dataLen - total);
//		ERROR_CHECK(ret, -1, "read");
//		total += ret;
//	}
//	return total;
//}

//int send_cycle(int socketFd, void *p, int dataLen){
//	char *data = (char *)p;
//	int total = 0;
//	int ret;
//	while(total < dataLen){
//		ret = write(socketFd, data + total, dataLen- total);
//		if(ret == -1){
//			return -1;
//		}
//		total += ret;
//	}
//	return 0;
//}
