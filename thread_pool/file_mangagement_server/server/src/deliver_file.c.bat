#include "../include/deliver_file.h"

int send_file(int socketFd, char *filename){
	Train_t train;
	memset(&train, 0, sizeof(Train_t));
	int ret;
	train.dataLen = strlen(filename);
	strcat(train.buf, filename);
	ret = send_cycle(socketFd, &train, 8 + train.dataLen);
	if(ret == -1){
		return -1;
	}
	int fd = open(filename, O_RDWR);
	ERROR_CHECK(fd, -1, "open");
	struct stat f_info;
	ret = fstat(fd, &f_info);
	ERROR_CHECK(ret, -1, "fstat");
	off_t offset = 0;
	char *map_ptr = mmap(NULL, f_info.st_size, PROT_READ, MAP_SHARED, fd, 0);
	ERROR_CHECK(map_ptr, (void *)-1, "mmap");
	train.dataLen = sizeof(f_info.st_size);
	memcpy(train.buf, &train.dataLen, train.dataLen);
	ret = send_cycle(socketFd, &train, 8 + train.dataLen);
	if(ret == -1){
		return -1;
	}
	if(offset < f_info.st_size){
		while(offset + BUFSIZE < f_info.st_size){
			memcpy(train.buf, map_ptr + offset, BUFSIZE);
			train.dataLen = BUFSIZE;
			ret = send_cycle(socketFd, &train, 8 + train.dataLen);
			if(ret == -1){
				return -1;
			}
			offset += BUFSIZE;
		}
		train.dataLen = f_info.st_size - offset;
	}
	else{
		train.dataLen = f_info.st_size;
	}
	memcpy(train.buf, map_ptr + offset, train.dataLen);
	ret = send_cycle(socketFd, &train, 8 + train.dataLen);
	if(ret == -1){
		return -1;
	}
	train.dataLen = 0;
	send_cycle(socketFd, &train, 8);

	ret = munmap(map_ptr, f_info.st_size);
	ERROR_CHECK(ret, -1, "munmap");
	return 0;


	//Train_t t;//定义发送数据的结构体
	//int ret;
	//t.dataLen=strlen(filename);
	//strcpy(t.buf,filename);
	////发送文件名给客户端
	//ret=send_cycle(new_fd,&t,8+t.dataLen);
	//if(-1==ret)
	//{
	//	return -1;
	//}
	//int fd=open(filename,O_RDONLY);
	////发送文件大小给客户端
	//struct stat buf;
	//fstat(fd,&buf);
	//t.dataLen=sizeof(buf.st_size);
	//memcpy(t.buf,&buf.st_size,sizeof(buf.st_size));
	//ret=send_cycle(new_fd,&t,8+t.dataLen);
	//if(-1==ret)
	//{
	//	return -1;
	//}
	//off_t ptrPos=0;//记录目前mmap指针的偏移
	//char *pMap=mmap(NULL,buf.st_size,PROT_READ,MAP_SHARED,fd,0);
	//ERROR_CHECK(pMap,(char*)-1,"mmap");	
	////发送文件内容给客户端
	//if(buf.st_size>1000)
	//{
	//	while(ptrPos+1000<buf.st_size)
	//	{
	//		memcpy(t.buf,pMap+ptrPos,sizeof(t.buf));
	//		t.dataLen=1000;
	//		ptrPos=ptrPos+1000;
	//		ret=send_cycle(new_fd,&t,8+t.dataLen);
	//		if(-1==ret)
	//		{
	//			return -1;
	//		}
	//	}
	//	t.dataLen=buf.st_size-ptrPos;
	//}else{
	//	t.dataLen=buf.st_size;
	//}
	////最后一个发送内容的列车
	//memcpy(t.buf,pMap+ptrPos,t.dataLen);
	//ret=send_cycle(new_fd,&t,8+t.dataLen);
	//if(-1==ret)
	//{
	//	return -1;
	//}
	//t.dataLen=0;
	//send_cycle(new_fd,&t,8);//发送文件发送结束标志
	//return 0;
}

int receive_file(int socketFd){
	int ret;
	Train_t train;
	memset(&train, 0, sizeof(Train_t));
	ret = read(socketFd, &train, sizeof(Train_t));
	printf("filename = %s, length of file = %d\n", train.buf, train.dataLen);
	int fileLen = train.dataLen;
	int fd;
	fd = open(train.buf, O_CREAT | O_RDWR, 0664);
	ERROR_CHECK(fd, -1, "open");
	while( (ret = recv_cycle(socketFd, &train, sizeof(Train_t))) > 0){
		if(train.flag == -1){
			break;
		}
		ret = write(fd, train.buf, train.dataLen);
		ERROR_CHECK(ret, -1, "write");
	}
	ERROR_CHECK(ret, -1, "read");

	close(fd);


	//int dataLen;
	//char buf[BUFSIZE]={0};
	//recv(socketFd,&dataLen,8,0);
	//recv(socketFd,buf,dataLen,0);
	//int fd;
	//fd=open(buf,O_CREAT|O_WRONLY,0666);
	//ERROR_CHECK(fd,-1,"open");
	//while(1){
	//	recv(socketFd,&dataLen,8,0);
	//	if(dataLen>0){
	//		recv(socketFd,buf,dataLen,0);
	//		write(fd,buf,dataLen);
	//	}
	//	else{
	//		break;
	//	}
	//}
	//close(fd);

	return 0;
}

int send_cycle(int fd, void *data, int dataLen){
	char *message = (char *)data;
	int total = 0;
	int ret;
	while(total < dataLen){
		ret = write(fd, message + total, dataLen - total);
		ERROR_CHECK(ret, -1, "write");
		total += ret;
	}
	return total;
}

int recv_cycle(int fd,void* message,int recvLen)
{
	char *p = (char *)message;
	int total=0;
	int ret;
	while(total<recvLen){
		ret=read(fd,p+total,recvLen-total);
		ERROR_CHECK(ret, -1, "read");
		total=total+ret;
	}
	return total;
}

