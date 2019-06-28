#ifndef __DEAL_SQL_H__
#define __DEAL_SQL_H__

#include "head.h"
#include "../include/pthread_and_queue.h"

#define HOST 		"localhost"
#define USERNAME 	"root"
#define PASSWD	 	"123456"
#define DATANAME 	"client"
#define PORT 		2222

int sql_init(MYSQL *, char *,char *, char *, char *, int);

int sql_log(Node_thread *);

int recv_log(int, int *);

int get_salt(char *, int);

int sql_insert_user(MYSQL *, char *, char *, char *);

int sql_select(MYSQL *, char *, char *, char *);

int sql_insert_file(MYSQL *, int , int , char *, char *, int , char *);

int sql_insert_folder(int);
#endif
