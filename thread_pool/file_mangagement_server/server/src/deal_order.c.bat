#include "../include/deal_order.h"
#include "../include/deliver_file.h"

//int main(int argc, char *argv[]){
//	ARGU_CHECK(argc, 3);
//	int ordNum;
//	char total_string[100] = {0};
//	strcpy(total_string, argv[1]);
//	strcat(total_string, " ");
//	strcat(total_string, argv[2]);
//	printf("total_string = %s\n", total_string);
//	char argument[50] = {0};
//	ordNum = explain_order(total_string, argument);
//	deal_order(ordNum, argument);
//	return 0;
//}

int sock_communication(int socketFd){
	char argument[BUFSIZE] = {0};
	char buf[BUFSIZE] = {0};
	int orderNum;
	int ret;
	ret = chdir(VISITOR_PATH);
	ERROR_CHECK(ret, -1, "chdir");
	char current_dir[BUFSIZE] = {0};
	strcat(current_dir, VISITOR_PATH);
	strcat(current_dir, "/");
	while(1){
		ret = read(socketFd, buf, BUFSIZE);
		printf("ret = %d\n", ret);
		if(ret == 0){
			printf("Byebye\n");
			break;
		}
		orderNum = explain_order(buf, argument);
		deal_order(orderNum, argument, socketFd, current_dir);
		printf("current_path = %s\n", current_dir);
		memset(argument, 0, BUFSIZE);
		memset(buf, 0, BUFSIZE);
	}
	return 0;
}

int explain_order(char *order, char *argument){
	char order_name[20] = {0};
	char order_argument[30] = {0};
	sscanf(order, "%s %s", order_name, order_argument);
	strcpy(argument, order_argument);
	if(strcmp(order_name, "cd") == 0){
		return 1;
	}
	if(strcmp(order_name, "ls") == 0){
		return 2;
	}
	if(strcmp(order_name, "puts") == 0){
		return 3;
	}
	if(strcmp(order_name, "gets") == 0){
		return 4;
	}
	if(strcmp(order_name, "remove") == 0){
		return 5;
	}
	if(strcmp(order_name, "pwd") == 0){
		return 6;
	}
	return -1;
}


int deal_order(int ordNum, char *ordArgument, int socketFd, char *current_dir){
	switch(ordNum){
		case 1: cd_fun(ordArgument, socketFd, current_dir);
				break;
		case 2: ls_fun(ordArgument, socketFd, current_dir);
				break;
		case 3: puts_fun(ordArgument, socketFd, current_dir); 
				break;
		case 4: gets_fun(ordArgument, socketFd, current_dir);
				break;
		case 5: remove_fun(ordArgument, socketFd, current_dir);
				break;
		case 6: pwd_fun(ordArgument, socketFd, current_dir);
				break;
		default: other_fun(socketFd); 
				break;
	}
	return 0;
}

int cd_fun(char *order_argument, int socketFd, char *current_dir){
	int ret;
	Train_t train;
	memset(&train, 0, sizeof(Train_t));
	char change_path[BUFSIZE] = {0};
	char client_path[BUFSIZE] = {0};
	char tmp[30] = {0};
	strcat(change_path, current_dir);
	strcat(change_path, order_argument);
	strcat(change_path, "/");
	train.flag = 1;
	sscanf(getcwd(NULL, 0), VISITOR_PATH"%s", tmp);
	if(strcmp(VISITOR_PATH, getcwd(NULL, 0)) != 0 && strlen(tmp) == 0){
		train.dataLen = -1;
	}
	else{
		if(ret == -1){
			train.dataLen = -1;
		}
		else{
			if(strcmp(current_dir, VISITOR_PATH) == 0 && strcmp(order_argument, "..") == 0){
				train.dataLen = -1;
			}
			
			else{
				memset(current_dir, 0, BUFSIZE);
				strcpy(current_dir, getcwd(NULL, 0));
				strcat(current_dir, "/");
				sscanf(current_dir, VISITOR_PATH"%s", tmp);
				strcat(train.buf, "/");
				strcpy(train.buf, tmp);
				train.dataLen = strlen(train.buf);
			}
		}
	}
	write(socketFd, &train, sizeof(Train_t));
	return 0;
}

int ls_fun(char *order_argument, int socketFd, char *current_dir){
	DIR *dp;
	dp = opendir(current_dir);
	struct dirent *dirp;
	char buf[BUFSIZE] = {0};
	while( (dirp = readdir(dp)) != NULL){
		if(strcmp(current_dir, VISITOR_PATH) == 0 && strcmp(dirp->d_name, "..") == 0){
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
	write(socketFd, &train, sizeof(Train_t));
	return 0;
}

int puts_fun(char *order_argument, int socketFd, char *current_dir){
//	printf("I'm puts, argument = %s\n", order_argument);
//	printf("I'm puts, socketFd = %d\n", socketFd);
	
	recv_file(socketFd, order_argument);
	return 0;
}

int gets_fun(char *order_argument, int socketFd, char *current_dir){
	//printf("I'm gets, argument = %s\n", order_argument);
	Train_t train;
	memset(&train, 0, sizeof(Train_t));
	train.flag = 4;
	strcpy(train.buf, order_argument);
	train.dataLen = strlen(order_argument);
	write(socketFd, &train, sizeof(train.buf));
	printf("buf = %s\n", train.buf);
	send_file(socketFd, order_argument);

	return 0;
}

int remove_fun(char *order_argument, int socketFd, char *current_dir){
	printf("I'm remove, argument = %s\n", order_argument);
	return 0;
}

int pwd_fun(char *order_argument, int socketFd, char *current_dir){
	printf("I'm pwd, argument = %s\n", order_argument);
	return 0;
}

int other_fun(int socketFd){
	Train_t train;
	memset(&train, 0, sizeof(Train_t));
	train.flag = -1;
	write(socketFd, &train, sizeof(Train_t));
	return 0;

}
