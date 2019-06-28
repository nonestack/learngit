#include "../include/sock_communication.h"
#include "../include/deliver_file.h"

#define PATH "/home/genius/Program/file_mangagement_server/client"

int sock_communication(int socketFd){
	char path[BUFSIZE] = {0};
	strcat(path, PATH);
	strcat(path, "/clond");
	chdir(path);
	Train_t train;
	memset(&train, 0, sizeof(Train_t));
	char order[20] = {0};
	char filename[20] = {0};
	char buf[BUFSIZE] = {0};
	char current_path[BUFSIZE] = {0};
	strcat(current_path, "/");
	while(1){
		//printf("start\n");
		//read(STDIN_FILENO, buf, BUFSIZE);
		scanf("%s", buf);
		sscanf(buf, "%s %s", order, filename);
		write(socketFd, buf, strlen(buf));
		printf("%s\n", buf);
		if(strcmp(order, "puts") == 0){
			receive_puts(socketFd, filename);
		}
		else{
			read(socketFd, &train, sizeof(Train_t));
			printf("train.flag = %d, train.buf = %s\n", train.flag, train.buf);
			//printf("buf = %s\n", train.buf);
			deal_train(socketFd, &train, current_path);
		}
		memset(&train, 0, sizeof(Train_t));
	}
	return 0;
}

int deal_train(int socketFd, Train_t *train, char *current_path){
	int flag = train->flag;
	switch(flag){
		case 1: receive_cd(train, current_path);
				break;
		case 2: receive_ls(train);
				break;
		//case 3: receive_puts(train);
		//		break;
		case 4: receive_gets(socketFd, train);
				break;
		case 5: receive_remove(train);
				break;
		case 6: receive_pwd(train);
				break;
		default: receive_other(train);
				break;
	}
	return 0;
}

int receive_cd(Train_t *train, char *current_path){
	if(train->dataLen == -1){
		printf("No such directory\n");
		return -1;
	}
	memset(current_path, 0, BUFSIZE);
	strcpy(current_path, train->buf);
	return 0;
}

int receive_ls(Train_t *train){
	//printf("i'm normal\n");
	printf("%s\n", train->buf);
	return 0;
}

int receive_puts(int socketFd, char *filename){
	int ret;
	ret = send_file(socketFd, filename);
	//have deal about error when ret = -1
	return 0;
}

int receive_gets(int socketFd, Train_t *train){
//	Train_t train;
//	memset(&train, 0, sizeof(Train_t));
//	read(socketFd, &train.flag, 4);
//	read(socketFd, &train.dataLen, 4);
//	read(socketFd, train.buf, train.dataLen);
	int ret;
	//printf("i'm here\n");
	//fflush(NULL);
	
	ret = recv_file(socketFd, train->buf);
	return 0;
}

int receive_remove(Train_t *train){
	return 0;
}

int receive_pwd(Train_t *train){
	return 0;
}

int receive_other(Train_t *train){
	return 0;
}
