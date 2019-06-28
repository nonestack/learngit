#include "../include/analyse_layer.h"

#define ORDER_SIZE 50
#define ARGU_SIZE  50

int analyse_order(int socketFd){
	Train_t train;
	char tmp[20] = {0};
	int ret;
	while(1){
		memset(&train, 0, sizeof(train));
		read(STDIN_FILENO, tmp, sizeof(tmp));

		strcat(train.buf, tmp);
		train.dataLen = strlen(train.buf);
		//write(socketFd, &train.flag, 4);				//write
		//write(socketFd, &train.dataLen, 4);				//write
		write(socketFd, train.buf, sizeof(train.buf));	//write
		char order[ORDER_SIZE] = {0};
		char argu[ARGU_SIZE] = {0};
		sscanf(train.buf, "%s %s", order, argu);
		int id;
		id = order_id(order);
		exec_order(socketFd, id, argu);
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

int exec_order(int socketFd, int id, char *argu){
	switch(id){
		case 1: cd_fun(socketFd, argu);
				break;
		case 2: ls_fun(socketFd, argu);
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

int cd_fun(int socketFd, char *argu){
	Train_t train;
	memset(&train, 0, sizeof(train));
	read(socketFd, &train.flag, 4);
	read(socketFd, &train.dataLen, 4);
	read(socketFd, train.buf, sizeof(train.buf));
	printf("dataLen = %d\n", train.dataLen);
	if(train.dataLen == -1){
		printf("No such directory\n");
		return -1;
	}
	return 0;
}

int ls_fun(int socketFd, char *argu){
	Train_t train;
	memset(&train, 0, sizeof(train));
	read(socketFd, &train.flag, 4);
	read(socketFd, &train.dataLen, 4);
	read(socketFd, train.buf, sizeof(train.buf));
	printf("%s\n", train.buf);
	return 0;
}

int puts_fun(int socketFd, char *argu){
	send_file(socketFd, argu);
	return 0;
}

int gets_fun(int socketFd, char *argu){
	recv_file(socketFd, argu);
	Train_t train;
	memset(&train, 0, sizeof(train));
	read(socketFd, &train.flag, 4);
	if(train. flag == 11){
		printf("download success\n");
	}
	return 0;
}

int remove_fun(int socketFd, char *argu){
	int ret;
	read(socketFd, &ret, 4);
	if(ret != -1){
		printf("delete success\n");
	}
	return 0;
}

int pwd_fun(int socketFd, char *argu){
	Train_t train;
	memset(&train, 0, sizeof(train));
	read(socketFd, &train.flag, 4);
	read(socketFd, &train.dataLen, 4);
	read(socketFd, train.buf, sizeof(train.buf));
	printf("%s\n", train.buf);
	return 0;
}

int other_fun(int socketFd, char *argu){
	return 0;
}
