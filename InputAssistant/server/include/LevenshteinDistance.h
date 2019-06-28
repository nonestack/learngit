#ifndef __LEVENSHTEINDISTANCE_H__
#define __LEVENSHTEINDISTANCE_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int min(int a, int b){
	return a < b ? a : b;
}

int minThr(int a, int b, int c){
	return min(a, min(b, c));
}

int editDistance(char *word1, int len1, char *word2,  int len2){
	int **d = (int **)calloc(len1 + 1, sizeof(int *));
	for(int i = 0; i < len1 + 1; ++i){
		d[i] = (int *)calloc(len2 + 1, sizeof(int));
	}
	int i, j;
	int flag, insert, delete, substitution;
	for(i = 0; i < len1 + 1; ++i){
		d[i][0] = i;
	}
	for(j = 0; j < len2 + 1; ++j){
		d[0][j] = j;
	}

	for(i = 1; i < len1 + 1; ++i){
		for(j = 1; j < len2 + 1; ++j){
#if 0
			//one of path in insert, delete and subtitution
			if(word1[i] == word2[j]){
				d[i][j] = d[i - 1][j - 1];
			}
			else{
				d[i][j] = d[i - 1][j - 1] + 1;
			}
#endif
#if 1
			flag = word1[i] == word2[j] ? 0 : 1;
			insert = d[i - 1][j] + 1;
			delete = d[i][j - 1] + 1;
			substitution = d[i - 1][j - 1] + flag;
			d[i][j] = minThr(insert, delete, substitution);
#endif
		}
	}
	printf("Levenshtein distance = %d\n", d[len1][len2]);
	free(d);
	return 0;
}

int test0(){
	char c1[] = "cafe";
	char c2[] = "coffee";
	editDistance(c1, strlen(c1), c2, strlen(c2));
	return 0;
}


#endif
