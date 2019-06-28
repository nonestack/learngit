#include "../include/analyse_layer.h"

#define PATH "/home/genius/Program/file_mangagement_server/server/clond"

#define ORDER_SIZE 50
#define ARGU_SIZE  50


int analyse_order(int socketFd){
	Train_t train;
	char order[ORDER_SIZE] = {0};
	char argu[ARGU_SIZE] = {0};
	char path[BUFSIZE] = {0};
	int ret;
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
		int id;
		id = order_id(order);
		exec_order(socketFd, id, argu, path);
	}
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

int exec_order(int socketFd, int id, char *argu, char *path){
	switch(id){
		case 1: cd_fun(socketFd, argu, path);
				break;
		case 2: ls_fun(socketFd, argu, path);
				break;
		case 3: puts_fun(socketFd, argu);
				break;
		case 4: gets_fun(socketFd, argu);
				break;
		case 5: remove_fun(socketFd, argu);
				break;
		case 6: pwd_fun(socketFd, argu);
				break;
		case 0: other_fun(socketFd, argu);
				break;
	}
	return 0;
}

int cd_fun(int socketFd, char *argu, char *path){
	//printf("%s\n", argu);
	int ret;
	Train_t train;
	memset(&train, 0, sizeof(Train_t));
	char change_path[BUFSIZE] = {0};
	char client_path[BUFSIZE] = {0};
	char tmp[30] = {0};
	strcat(change_path, path);
	strcat(change_path, argu);
	strcat(change_path, "/");
	ret = chdir(change_path);
	ERROR_CHECK(ret, -1, "chdir");
	train.flag = 1;
	sscanf(getcwd(NULL, 0), PATH"%s", tmp);
	if(strcmp(PATH, getcwd(NULL, 0)) != 0 && strlen(tmp) == 0){
		train.dataLen = -1;
	}
	else{
		if(ret == -1){
			train.dataLen = -1;
		}
		else{
			if(strcmp(path, PATH) == 0 && strcmp(argu, "..") == 0){
				train.dataLen = -1;
			}
			
			else{
				memset(path, 0, BUFSIZE);
				strcpy(path, getcwd(NULL, 0));
				strcat(path, "/");
				sscanf(path, PATH"%s", tmp);
				strcat(train.buf, "/");
				strcpy(train.buf, tmp);
				//printf("i'm client_path = %s\n", train.buf);
				train.dataLen = strlen(train.buf);
			}
		}
	}
	write(socketFd, &train.flag, 4);
	write(socketFd, &train.dataLen, 4);
	write(socketFd, train.buf, sizeof(train.buf));
	return 0;
}

int ls_fun(int socketFd, char *argu, char *path){
	DIR *dp;
	dp = opendir(path);
	struct dirent *dirp;
	char buf[BUFSIZE] = {0};
	while( (dirp = readdir(dp)) != NULL){
		if(strcmp(path, PATH) == 0 && strcmp(dirp->d_name, "..") == 0){
			continue;
		}
		strcat(buf, dirp->d_name);
		strcat(buf, " ");
	}
	Train_t train;
	memset(&train, 0, sizeof(Train_t));
	train.flag = 2;
	strcpy(train.buf, buf);
	train.dataLen = strlen(train.buf);
	write(socketFd, &train.flag, 4);
	write(socketFd, &train.dataLen, 4);
	write(socketFd, train.buf, sizeof(train.buf));
	return 0;
}

int puts_fun(int socketFd, char *argu){
	recv_file(socketFd, argu);
	return 0;
}

int gets_fun(int socketFd, char *argu){
	send_file(socketFd, argu);
	Train_t train;
	memset(&train, 0, sizeof(train));
	train.flag = 11;	//repesent success
	write(socketFd, &train.flag, 4);
	return 0;
}

int remove_fun(int socketFd, char *argu){
	int ret;
	ret = unlink(argu);
	ERROR_CHECK(ret, -1, "unlink");
	write(socketFd, &ret, 4);
	return 0;
}

int pwd_fun(int socketFd, char *argu){
	Train_t train;
	memset(&train, 0, sizeof(train));
	char tmp[BUFSIZE] = {0};
	strcat(tmp, getcwd(NULL, 0));
	sscanf(tmp, PATH"%s", train.buf);
	write(socketFd, &train.flag, 4);
	write(socketFd, &train.dataLen, 4);
	write(socketFd, train.buf, sizeof(train.buf));
	return 0;
}

int other_fun(int socketFd, char *argu){
	return 0;
}
