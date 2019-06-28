#include "../include/deal_user.h"


int send_log(int socketFd){
	Train_t train;
	memset(&train, 0, sizeof(Train_t));
	char sign;
	char user[30] = {0};
	char key[30] = {0};
	char passwd[30] = {0};
	char salt[3] = {0};
	//judge user tend to sign in or sign up
	do{
		printf("Sign in or sign up[i/u]?:");
		fflush(NULL);
		scanf("%c", &sign);
		fflush(NULL);
	}while(sign != 'i' && sign != 'I' && sign != 'u' && sign != 'U');
	//send sign of sign in or sign up
	write(socketFd, &sign, 1);
	printf("username : ");
	fflush(NULL);
	scanf("%s", user);
	//send username
	strcpy(train.buf, user);
	train.dataLen = strlen(train.buf);
	write(socketFd, &train.flag, 4);
	write(socketFd, &train.dataLen, 4);
	write(socketFd, train.buf, train.dataLen);
	memset(&train, 0, sizeof(Train_t));
	//receive salt from server
	read(socketFd, &train.flag, 4);
	read(socketFd, &train.dataLen, 4);
	read(socketFd, train.buf, train.dataLen);
	strcpy(salt, train.buf);
	memset(&train, 0, sizeof(train));
	printf("salt = %s\n", salt);
	//put in passwd
	if(sign == 'i' || sign == 'I'){
		printf("password : ");
		fflush(NULL);
		scanf("%s", key);
	}
	else{
		char key_again[30] = {0};
		do{
			printf("password : ");
			fflush(NULL);
			scanf("%s", key);
			printf("put in password again : ");
			fflush(NULL);
			scanf("%s", key_again);
		}while(strcmp(key, key_again) != 0);
	}
	//encrypt password
	strcpy(passwd, crypt(key, salt));
	printf("encrypting passwd = %s\n", passwd);
	//send passwd
	strcpy(train.buf, passwd);
	train.dataLen = strlen(train.buf);
	write(socketFd, &train.flag, 4);
	write(socketFd, &train.dataLen, 4);
	write(socketFd, train.buf, train.dataLen);
	memset(&train, 0, sizeof(Train_t));
	//receive sign of success or failed
	read(socketFd, &train.flag, 4);
	if(train.flag == -1){
		return -1;
	}
	return 0;
}
