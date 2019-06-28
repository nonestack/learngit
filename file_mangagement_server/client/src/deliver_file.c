#include "../include/deliver_file.h"
#include "../include/md5.h"

int send_file(int socketFd, char *filename){
	Train_t train;
	memset(&train, 0, sizeof(Train_t));
	int fd;
	int ret;
	int size;
	int total = 0;
	//open file
	fd = open(filename, O_RDWR);
	ERROR_CHECK(fd, -1, "open");
	//get size of file
	struct stat buf;
	memset(&buf, 0, sizeof(struct stat));
	ret = fstat(fd, &buf);
	ERROR_CHECK(ret, -1, "fstat");
	size = buf.st_size;
	//send size of file
	write(socketFd, &size, 4);
	//receive sign of remote size of file
	read(socketFd, &train.flag, 4);
	if(train.flag != 13){
		printf("sign error\n");
		return -1;
	}
	read(socketFd, &train.dataLen, 4);
	//judge site of character in file
	if(train.dataLen > 0){
		ret = lseek(fd, train.dataLen, SEEK_SET);
		total += train.dataLen;
		ERROR_CHECK(ret, -1, "lseek");
	}
	while((ret = read(fd, train.buf, sizeof(train.buf)))){
		train.dataLen = ret;
		write(socketFd,&train.flag, 4);
		write(socketFd,&train.dataLen, 4);
		write(socketFd,train.buf, train.dataLen);
		memset(&train, 0, sizeof(train));
	}
	train.flag = -1;
	write(socketFd, &train.flag, 4);
	close(fd);
	return 0;
}

int recv_file(int socketFd, char *filename){
	Train_t train;
	memset(&train, 0, sizeof(Train_t));
	int fd;
	int ret;
	int size;
	int total = 0;
	off_t offset = 0;
	//receive size of file
	read(socketFd, &size, 4);
	//judge file whether existence
	fd = open(filename, O_RDWR);
	if(fd == -1){
		fd = open(filename, O_CREAT | O_RDWR, 0664);
		ERROR_CHECK(fd, -1, "open");
	}
	else{
		//write size of existence file if file existence
		struct stat buf;
		memset(&buf, 0, sizeof(struct stat));
		ret = fstat(fd, &buf);
		ERROR_CHECK(ret, -1, "fstat");
		offset = buf.st_size;
		total += offset;
		ret = lseek(fd, 0, SEEK_END);
	}
	printf("size of file before = %d\n", total);
	//send size of file existence to send pointer
	train.flag = 13;		//13 token size of locaton file
	train.dataLen = offset;
	write(socketFd, &train.flag, 4);
	write(socketFd, &train.dataLen, 4);
	while(1){
		ret = read(socketFd, &train.flag, 4);
		if(train.flag == -1){
			break;
		}
		ret = read(socketFd, &train.dataLen, 4);
		ret = read(socketFd, train.buf, train.dataLen);
		write(fd, train.buf, train.dataLen);
		total += train.dataLen;
		printf("\r%.2lf%%    ", total * 100.0 / size);
		memset(&train, 0, sizeof(Train_t));
	}
	printf("\n");
	printf("total = %d, size of file = %d\n", total, size);
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

int send_md5(int socketFd, char *filename){
	//get md5 
	char md5[33] = {0};
	int fd = open(filename, O_RDWR);
	get_md5(fd, md5);
	Train_t train;
	memset(&train, 0, sizeof(train));
	strcpy(train.buf, md5);
	train.dataLen = strlen(train.buf);
	//send md5 of argu to server
	write(socketFd, &train.flag, 4);
	write(socketFd, &train.dataLen, 4);
	write(socketFd, train.buf, train.dataLen);
	memset(&train, 0, sizeof(train));
	//get flag of file whether send
	read(socketFd, &train.flag, 4);
	return train.flag;
}
