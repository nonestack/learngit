#include "head.h"

int main(){
	char ch1[20] = {0};
	char ch2[20] = {0};
	sscanf("Hello", "%s%s", ch1, ch2);
	printf("ch1 = %s, length of ch1 = %lu\nch2 = %s, length of ch2 = %lu\n", ch1, strlen(ch1), ch2, strlen(ch2));
	return 0;
}
