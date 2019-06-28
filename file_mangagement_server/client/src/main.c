#include "../include/deal_user.h"
#include "../include/analyse_layer.h"

#define PATH "/home/genius/Program/file_mangagement_server/client/clond/"

int main(int argc, char *argv[]){
	ARGU_CHECK(argc, 3);
	int socketFd;
	int ret;
	ret = chdir(PATH);
	socketFd = socket(AF_INET, SOCK_STREAM, 0);
	ERROR_CHECK(socketFd, -1, "socket");
	struct sockaddr_in serAddr;
	int addrlen = sizeof(struct sockaddr_in);
	memset(&serAddr, 0, addrlen);
	serAddr.sin_family = AF_INET;
	serAddr.sin_addr.s_addr = inet_addr(argv[1]);
	serAddr.sin_port = htons(atoi(argv[2]));
	ret = connect(socketFd, (struct sockaddr *)&serAddr, addrlen);
	ERROR_CHECK(ret, -1, "connect");
	printf("connect success\n");

	//sign in or sign up
	ret = send_log(socketFd);
	if(ret == -1){
		printf("password error\n");
		return -1;
	}
	analyse_order(socketFd);

	return 0;
}
