#include "head.h"

int main(int argc, char *argv[]){
	ARGU_CHECK(argc, 2);
	int fd = open(argv[1], O_RDWR);
	ERROR_CHECK(fd, -1, "open");
	MD5_CTX ctx;
	unsigned char md[16];
	char buf[33]={0};
	char tmp[3]={0};
	char data[BUFSIZE] = {0};
	int i;
	int ret;
	
	MD5_Init(&ctx);
	while(ret = read(fd, data, sizeof(data))){
		MD5_Update(&ctx, data, ret);
	}
	MD5_Final(md,&ctx);
	
	for( i=0; i<16; i++ ){
		sprintf(tmp,"%02x",md[i]);
	    strcat(buf,tmp);
	}
	printf("%s\n",buf);
	return 0;
}
