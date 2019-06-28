#include "../include/deliver_file.h"

int send_file(int socketFd, char *filename){
	Train_t train;
	memset(&train, 0, sizeof(Train_t));
	int ret;
	strcat(train.buf, filename);
	ret = strlen(train.buf);
	ret = write(socketFd, &train, 8 + ret);
	printf("ret = %d\n", ret);
	//printf("filename = %s\n", train.buf);
	int fd = open(filename, O_RDWR);
	ERROR_CHECK(fd, -1, "open");

	struct stat f_info;
	ret = fstat(fd, &f_info);
	ERROR_CHECK(ret, -1, "fstat");

	off_t offset = 0;
	train.dataLen = f_info.st_size;
	printf("length of file = %d\n", train.dataLen);
	printf("filename = %s\n", train.buf);
	if(ret == -1){
		return -1;
	}

	while( (ret = read(fd, train.buf, BUFSIZE)) > 0){
		train.dataLen = ret;
		ret = send_cycle(socketFd, &train, sizeof(Train_t));
		memset(&train, 0, sizeof(Train_t));
	}
	train.flag = -1;
	send_cycle(socketFd, &train, sizeof(Train_t));


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
	printf("read is OK\n");
	ERROR_CHECK(ret, -1, "read");
	int fd;
	fd = open(train.buf, O_CREAT | O_RDWR, 0664);
	ERROR_CHECK(fd, -1, "open");
	//fd = open(train.buf, O_RDONLY);
	//if(fd != -1){
	//	char tmp[BUFSIZE] = {0};
	//	printf("overwrite '%s'?:");
	//	fflush(STDOUT_FILENO);
	//	scanf("%s", tmp);
	//	if(strcmp(tmp, "y") == 0 || strcmp(tmp, "Y") == 0){
	//	}
	//}
	//printf("i'm while before\n");
	while( (ret = read(socketFd, &train, sizeof(Train_t))) > 0){
		if(train.dataLen == 0){
			break;
		}
		ret = write(fd, train.buf, ret - 8);
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
