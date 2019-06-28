#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>


#include <iostream>
#include <string>
#include <vector>
#include <iterator>

using namespace std;

void test(size_t);

int main(int argc, char *argv[]){
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in _addr;
	memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(atoi(argv[2]));
	_addr.sin_addr.s_addr = inet_addr(argv[1]);

	int ret;
	ret = connect(sockfd, (struct sockaddr *)&_addr, sizeof(_addr));
	if(ret == -1){
		perror("connect");
		return -1;
	}

	char tmp[1024] = {0};
	read(sockfd, tmp, sizeof(tmp));
	cout << "ok" << endl;
	cout << tmp << endl;

	test(sockfd);


	close(sockfd);
	return 0;
}

void test(size_t sockfd){
	char buf[1024] = {0};
	while(1){
		memset(buf, 0, sizeof(buf));
		scanf("%s", buf);
		//strcpy(buf, "Hello, I'm client");
		cout << buf << endl;
		write(sockfd, buf, strlen(buf));
	}
}