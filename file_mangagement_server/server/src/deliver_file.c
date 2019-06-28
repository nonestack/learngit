#include "../include/deliver_file.h"
#include "../include/pthread_and_queue.h"
#include "../include/deal_sql.h"

int send_file(Node_thread *pNode, char *md5){
	Train_t train;
	memset(&train, 0, sizeof(Train_t));
	int socketFd = pNode->childFd;
	int fd;
	int ret;
	int size;
	int total = 0;
	//open file
	fd = open(md5, O_RDWR);
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

int recv_file(Node_thread *pNode, char *filename, char *md5){
	Train_t train;
	memset(&train, 0, sizeof(Train_t));
	int socketFd = pNode->childFd;
	int fd;
	int ret;
	int size;
	int total = 0;
	off_t offset = 0;
	//receive size of file
	read(socketFd, &size, 4);
	//judge file whether existence
	fd = open(md5,  O_RDWR);
	if(fd == -1){
		fd = open(md5, O_CREAT | O_RDWR, 0664);
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
		ERROR_CHECK(ret, -1, "lseek");
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

int recv_md5(Node_thread *pNode, char *filename, char *buf){
	int socketFd = pNode->childFd;
	Train_t train;
	memset(&train, 0, sizeof(train));
	char md5[33] = {0};
	int ret;
	//receive md5 of file
	read(socketFd, &train.flag, 4);
	read(socketFd, &train.dataLen, 4);
	read(socketFd, train.buf, train.dataLen);
	strcpy(md5, train.buf);
	strcpy(buf, md5);
	memset(&train, 0, sizeof(train));
	//open database and get result of select
	MYSQL *conn;
	MYSQL_RES *res_ptr;
	MYSQL_ROW my_row;
	conn = mysql_init(NULL);
	mysql_real_connect(conn, HOST, USERNAME, PASSWD, DATANAME, 2222, NULL, 0);
	sql_select(conn, "file_information", "md5", md5);
	res_ptr = mysql_store_result(conn);
	if(my_row = mysql_fetch_row(res_ptr)){
		train.flag = 1;
		ret = 1;
		sql_insert_file(conn, pNode->path, pNode->userid, filename, "f", atoi(my_row[0]), md5);
	}
	else{
		train.flag = -1;
		ret = -1;
		sql_insert_file(conn, pNode->path, pNode->userid, filename, "f", 0, md5);
	}
	mysql_free_result(res_ptr);
	write(socketFd, &train.flag, 4);
	

	mysql_close(conn);
	return ret;
}
