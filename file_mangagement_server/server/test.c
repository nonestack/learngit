#include "head.h"

int main(){
	struct stat buf;
	int fd;
	fd = open("file1", O_CREAT | O_RDWR, 0664);

	int ret;
	ret = lseek(fd, 15, SEEK_SET);
	ERROR_CHECK(ret, -1, "lseek");
	write(fd, "hello", 5);
	//char *map_ptr = mmap(NULL, 10, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
	//ERROR_CHECK(map_ptr, (void *)-1, "mmap");
	//sscanf("helloWord", "%s", map_ptr);
	//printf("hello\n");
	//munmap(map_ptr, 10);
	close(fd);
	return 0;
}
