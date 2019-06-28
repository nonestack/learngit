#include "../include/analyse_layer.h"
#include "../include/deal_sql.h"
#include "../include/pthread_and_queue.h"



#define ORDER_SIZE 50
#define ARGU_SIZE  50


int analyse_order(Node_thread *pNode){
	int socketFd = pNode->childFd;
	struct sockaddr_in cliAddr;
	memcpy(&cliAddr, &pNode->addrInfo, sizeof(struct sockaddr_in));
	Train_t train;
	char order[ORDER_SIZE] = {0};
	char argu[ARGU_SIZE] = {0};
	char path[BUFSIZE] = {0};
	int ret;
	int fd;
	time_t now;
	char now_time[BUFSIZE] = {0};
	ret = chdir(PATH);
	ERROR_CHECK(ret, -1, "chdir");
	strcat(path, PATH);
	strcat(path, "/");
	while(1){
		memset(&train, 0, sizeof(train));
		//ret = read(socketFd, &train.flag, 4);					//read
		//ret = read(socketFd, &train.dataLen, 4);				//read
		ret = read(socketFd, train.buf, sizeof(train.buf));	//read
		if(ret == 0){
			printf("Byebye\n");
			break;
		}
		sscanf(train.buf, "%s %s", order, argu);
		//write Log
		now = time(NULL);
		fd = open(LOG_NAME, O_RDWR);
		ERROR_CHECK(fd, -1, "open");
		ret = lseek(fd, 0, SEEK_END);
		ERROR_CHECK(ret, -1, "lseek");
		write(fd, inet_ntoa(pNode->addrInfo.sin_addr), strlen(inet_ntoa(pNode->addrInfo.sin_addr)));
		write(fd, ": ", 1);
		write(fd, train.buf, strlen(train.buf));
		write(fd, " ", 1);
		strcpy(now_time, ctime(&now));
		write(fd, now_time, strlen(now_time));
		write(fd, "\n", 1);
		close(fd);
		
		//execute order
		int id;
		id = order_id(order);
		exec_order(pNode, id, argu, path);
	}
	//write Log
	now = time(NULL);
	fd = open(LOG_NAME, O_RDWR);
	ERROR_CHECK(fd, -1, "open");
	ret = lseek(fd, 0, SEEK_END);
	ERROR_CHECK(ret, -1, "lseek");
	write(fd, inet_ntoa(pNode->addrInfo.sin_addr), strlen(inet_ntoa(pNode->addrInfo.sin_addr)));
	write(fd, ": Log out ", 10);
	strcpy(now_time, ctime(&now));
	write(fd, now_time, strlen(now_time));
	write(fd, "\n", 1);
	close(fd);

	return 0;
}

int order_id(char *order){
	if(strcmp(order, "cd") == 0){
		return 1;
	}
	if(strcmp(order, "ls") == 0){
		return 2;
	}
	if(strcmp(order, "puts") == 0){
		return 3;
	}
	if(strcmp(order, "gets") == 0){
		return 4;
	}
	if(strcmp(order, "remove") == 0){
		return 5;
	}
	if(strcmp(order, "pwd") == 0){
		return 6;
	}
	return 0;
}

int exec_order(Node_thread *pNode, int id, char *argu, char *path){
	int socketFd = pNode->childFd;
	switch(id){
		case 1: cd_fun(pNode, argu, path);
				break;
		case 2: ls_fun(pNode, argu, path);
				break;
		case 3: puts_fun(pNode, argu);
				break;
		case 4: gets_fun(pNode, argu);
				break;
		case 5: remove_fun(pNode, argu);
				break;
		case 6: pwd_fun(pNode, argu);
				break;
		case 0: other_fun(socketFd, argu);
				break;
	}
	return 0;
}

int cd_fun(Node_thread *pNode, char *argu, char *path){
	Train_t train;
	memset(&train, 0, sizeof(train));
	int socketFd = pNode->childFd;
	MYSQL *conn;
	MYSQL_RES *res_ptr;
	MYSQL_ROW my_row;
	conn = mysql_init(NULL);
	if(NULL == mysql_real_connect(conn, "localhost", "root", "123456", "client", 2222, NULL, 0)){
		printf("connect error\n");
		return -1;
	}
	//mysql_real_connect(conn, "localhost", "root", "123456", "    client", 2222, NULL, 0); 
	
	char my_query[BUFSIZE] = {0};
	if(strcmp(argu, "..") == 0){
		if(pNode->path == 0){
			train.flag = -1;
			write(socketFd, &train.flag, 4);
		}
		else{
			sprintf(my_query, "SELECT * FROM file_information WHERE code = %d", pNode->path);
			printf("%s\n", my_query);
			mysql_query(conn, my_query);
			res_ptr = mysql_store_result(conn);
			if(res_ptr == NULL){
				//printf("%s\n", mysql_error(conn));
				printf("%s\n", mysql_error(conn));
				return -1;
			}
			train.flag = 1;
			my_row = mysql_fetch_row(res_ptr);
			pNode->path = atoi(my_row[1]);
			write(socketFd, &train.flag, 4);
			memset(pNode->cpath, 0, sizeof(pNode->cpath));
			strcpy(pNode->cpath, "/");
		}
		memset(&train, 0, sizeof(train));
	}
	else if(strcmp(argu, ".") == 0){
		train.flag = 1;
		write(socketFd, &train.flag, 4);
		memset(&train, 0, sizeof(train));
	}
	else{
		sprintf(my_query, "SELECT * FROM file_information WHERE precode = %d AND userid = %d AND filetype = 'd' AND filename = '%s'", pNode->path, pNode->userid, argu);
		printf("%s\n", my_query);
		mysql_query(conn, my_query);
		res_ptr = mysql_store_result(conn);
		if(res_ptr == NULL){
			//printf("%s\n", mysql_error(conn));
			train.flag = -1;
		}
		else{
			my_row = mysql_fetch_row(res_ptr);
			pNode->path = atoi(my_row[0]);
			train.flag = 1;
			strcat(pNode->cpath, argu);
		}
		write(socketFd, &train.flag, 4);
		memset(&train, 0, sizeof(train));
		printf("path = %d\n", pNode->path);
		mysql_free_result(res_ptr);
	}
	mysql_close(conn);

	//int socketFd = pNode->childFd;
	////printf("%s\n", argu);
	//int ret;
	//Train_t train;
	//memset(&train, 0, sizeof(Train_t));
	//char change_path[BUFSIZE] = {0};
	//char client_path[BUFSIZE] = {0};
	//char tmp[30] = {0};
	//strcat(change_path, path);
	//strcat(change_path, argu);
	//strcat(change_path, "/");
	//ret = chdir(change_path);
	//ERROR_CHECK(ret, -1, "chdir");
	//train.flag = 1;
	//sscanf(getcwd(NULL, 0), PATH"%s", tmp);
	//if(strcmp(PATH, getcwd(NULL, 0)) != 0 && strlen(tmp) == 0){
	//	train.dataLen = -1;
	//}
	//else{
	//	if(ret == -1){
	//		train.dataLen = -1;
	//	}
	//	else{
	//		if(strcmp(path, PATH) == 0 && strcmp(argu, "..") == 0){
	//			train.dataLen = -1;
	//		}
	//		
	//		else{
	//			memset(path, 0, BUFSIZE);
	//			strcpy(path, getcwd(NULL, 0));
	//			strcat(path, "/");
	//			sscanf(path, PATH"%s", tmp);
	//			strcat(train.buf, "/");
	//			strcpy(train.buf, tmp);
	//			//printf("i'm client_path = %s\n", train.buf);
	//			train.dataLen = strlen(train.buf);
	//		}
	//	}
	//}
	//write(socketFd, &train.flag, 4);
	//write(socketFd, &train.dataLen, 4);
	//write(socketFd, train.buf, sizeof(train.buf));
	return 0;
}

int ls_fun(Node_thread *pNode, char *argu, char *path){
	int ret;
	int socketFd = pNode->childFd;
	Train_t train;
	memset(&train, 0, sizeof(train));
	char files[BUFSIZE] = {0};
	if(pNode->path == 0){
		strcat(files, ". ");
	}
	else{
		strcat(files, ". ..");
	}
	MYSQL *conn;
	MYSQL_RES *res_ptr;
	MYSQL_ROW my_row;
	conn = mysql_init(NULL);
	if(NULL == mysql_real_connect(conn, "localhost", "root", "123456", "client", 2222, NULL, 0)){
		printf("connect error\n");
		return -1;
	}
	char id[BUFSIZE] = {0};
	sprintf(id, "%d", pNode->userid);
	//sql_select(conn, "file_information", "userid", id);
	sql_select(conn, "file_information", "userid", id);
	//printf("%d\n%s\n", ret, mysql_error(conn));
	//printf("%d\n",mysql_field_count(conn));
	res_ptr = mysql_store_result(conn);
	if(res_ptr == NULL){
		printf("res_ptr == NULL\n%s\n", mysql_error(conn));
		return -1;
	}
	while(my_row = mysql_fetch_row(res_ptr)){
		if(pNode->path == atoi(my_row[1])){
			strcat(files, my_row[3]);
			strcat(files, " ");
		}	
	}
	strcpy(train.buf, files);
	train.dataLen = strlen(train.buf);
	write(socketFd, &train.flag, 4);
	write(socketFd, &train.dataLen, 4);
	write(socketFd, train.buf, train.dataLen);
	mysql_free_result(res_ptr);
	mysql_close(conn);

	//DIR *dp;
	//dp = opendir(path);
	//struct dirent *dirp;
	//char buf[BUFSIZE] = {0};
	//while( (dirp = readdir(dp)) != NULL){
	//	if(strcmp(path, PATH) == 0 && strcmp(dirp->d_name, "..") == 0){
	//		continue;
	//	}
	//	strcat(buf, dirp->d_name);
	//	strcat(buf, " ");
	//}
	//Train_t train;
	//memset(&train, 0, sizeof(Train_t));
	//train.flag = 2;
	//strcpy(train.buf, buf);
	//train.dataLen = strlen(train.buf);
	//write(socketFd, &train.flag, 4);
	//write(socketFd, &train.dataLen, 4);
	//write(socketFd, train.buf, sizeof(train.buf));
	return 0;
}

int puts_fun(Node_thread *pNode, char *argu){
	int ret;
	char md5[33] = {0};
	ret = recv_md5(pNode, argu, md5);
	if(ret == -1){
		recv_file(pNode, argu, md5);
	}
	return 0;
}

int gets_fun(Node_thread *pNode, char *argu){
	int flag = -1;
	int socketFd = pNode->childFd;
	//select file argu from database
	char md5[33] = {0};
	MYSQL *conn;
	MYSQL_RES *res_ptr;
	MYSQL_ROW my_row;
	conn = mysql_init(NULL);
	mysql_real_connect(conn, "localhost", "root", "123456", "client", 2222, NULL, 0);
	//need a function to receive two argument of condection to prevent conflict about path
	sql_select(conn, "file_information", "filename", argu);
	//my_row = mysql_fetch_row(res_ptr);
	res_ptr = mysql_store_result(conn);
	printf("%d\n", pNode->userid);
	while(my_row = mysql_fetch_row(res_ptr)){
		if(pNode->userid == atoi(my_row[2])){
			flag = 1;
			strcpy(md5, my_row[6]);
			break;
		}
	}
	write(socketFd, &flag, 4);
	printf("md5 = %s\n", md5);
	if(flag == 1){
		send_file(pNode, md5);
		Train_t train;
		memset(&train, 0, sizeof(train));
		train.flag = 11;	//repesent success
		write(socketFd, &train.flag, 4);
	}
	else{
	}
	mysql_free_result(res_ptr);
	mysql_close(conn);
	return 0;
}

int remove_fun(Node_thread *pNode, char *argu){
	Train_t train;
	memset(&train, 0, sizeof(train));
	int ret;
	int socketFd = pNode->childFd;
	MYSQL *conn;
	MYSQL_RES *res_ptr;
	MYSQL_ROW my_row;
	conn = mysql_init(NULL);
	mysql_real_connect(conn, "localhost", "root", "123456", "client", 2222, NULL, 0);
	char my_query[BUFSIZE] = {0};
	sprintf(my_query, "SELECT * FROM file_information WHERE userid = %d AND filetype = 'f' AND precode = %d AND filename = '%s'", pNode->userid, pNode->path, argu);
	mysql_query(conn, my_query);
	res_ptr = mysql_store_result(conn);
	if((my_row = mysql_fetch_row(res_ptr)) == NULL){
		train.flag = -1;
		mysql_free_result(res_ptr);
	}
	else{
		train.flag = 1;
		ret = unlink(my_row[6]);
		ERROR_CHECK(ret, -1, "unlink");
		int id = atoi(my_row[0]);
		memset(my_query, 0, sizeof(my_query));
		sprintf(my_query, "DELETE FROM file_information WHERE code = %d", id);
		mysql_free_result(res_ptr);
		mysql_query(conn, my_query);
	}
	mysql_close(conn);
	write(socketFd, &train.flag, 4);
	memset(&train, 0, sizeof(train));
	return 0;
}

int pwd_fun(Node_thread *pNode, char *argu){
	Train_t train;
	memset(&train, 0, sizeof(train));
	int socketFd = pNode->childFd;
	strcat(train.buf, pNode->cpath);
	train.dataLen = strlen(train.buf);
	write(socketFd, &train.flag, 4);
	write(socketFd, &train.dataLen, 4);
	write(socketFd, train.buf, train.dataLen);
	return 0;
}

int other_fun(int socketFd, char *argu){
	return 0;
}
