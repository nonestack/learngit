#include "../include/md5.h"

int get_md5(int fd, char *buf){
	MD5_CTX ctx;
	unsigned char md[16];
	memset(buf, 0, 33);
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
	return 0;

}
