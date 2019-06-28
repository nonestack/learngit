#include "../include/deal_description.h"


int extract_conf(char *filename, char *ip, char *port, int *threadNum, int *capacity){
	int fd;
	fd = open(filename, O_RDWR);
	ERROR_CHECK(fd, -1, "fd");
	char buf[BUFSIZE] = {0};
	char tmp;
	int ret;
	char ip_name[20] = {0};
	char port_name[20] = {0};
	char threadNum_name[20] = {0};
	char capacity_name[20] = {0};
	ret = read(fd, buf, BUFSIZE);
	ERROR_CHECK(ret, -1, "read");
	sscanf(buf, "%s = %s\n%s = %s\n%s = %d\n%s = %d", ip_name, ip, port_name, port, threadNum_name, threadNum, capacity_name, capacity);
	return 0;
}

int tcp_init(int *socketFd, char *ip, char *port){
	int ftmp;
	int ret;
	ftmp = socket(AF_INET, SOCK_STREAM, 0);
	ERROR_CHECK(ftmp, -1, "socket");
	int reuse = 1;
	ret = setsockopt(ftmp, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuse, sizeof(int));
	ERROR_CHECK(ret, -1, "setsockopt");
	struct sockaddr_in serAddr;
	int addrlen = sizeof(serAddr);
	memset(&serAddr, 0, addrlen);
	serAddr.sin_family = AF_INET;
	serAddr.sin_addr.s_addr = inet_addr(ip);
	serAddr.sin_port = htons(atoi(port));
	ret = bind(ftmp, (struct sockaddr *)&serAddr, addrlen);
	ERROR_CHECK(ret, -1, "bind");
	ret = listen(ftmp, 10);
	*socketFd = ftmp;
	return 0;
}
