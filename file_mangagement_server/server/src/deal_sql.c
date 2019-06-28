#include "../include/deal_sql.h"


int sql_log(Node_thread *pNode){
	int ret;
	int flag;
	//get userid for Node_thread
	ret = recv_log(pNode->childFd, &flag);
	pNode->userid = ret;
	if(flag == 1){
		sql_insert_folder(pNode->userid);
	}
	printf("%d\n",pNode->userid);
	return 0;
}

int recv_log(int socketFd, int *flag){
	Train_t train;
	memset(&train, 0, sizeof(Train_t));
	char salt[3] = {0};
	char sign;
	char user[30] = {0};
	char passwd[30] = {0};
	int userid;
	//send sign of sign in or sign up
	read(socketFd, &sign, 1);
	//receive username
	read(socketFd, &train.flag, 4);
	read(socketFd, &train.dataLen, 4);
	read(socketFd, train.buf, train.dataLen);
	strcpy(user, train.buf);
	memset(&train, 0, sizeof(Train_t));
	//connect databases
	MYSQL *conn;
	conn = mysql_init(NULL);
	ERROR_CHECK(conn, NULL, "mysql_init");
	if(NULL == mysql_real_connect(conn, HOST, USERNAME, PASSWD, DATANAME, PORT, NULL, 0)){
		perror("mysql_real_connect");
		return -1;
	}
	//judge sign in or sign up
	if(sign == 'i' || sign == 'I'){
		*flag = 0;
		MYSQL_RES *res_ptr;
		MYSQL_ROW my_row;
		sql_select(conn, "user_information", "name", user);
		//printf("username = %s\n", user);
		//printf("password = %s\n", passwd);
		//get return of select
		res_ptr = mysql_store_result(conn);
		if(res_ptr == NULL){
			printf("mysql_store_result error\n");
			return -1;
		}
		my_row = mysql_fetch_row(res_ptr);
		//get salt 
		strcpy(salt, my_row[3]);
		mysql_free_result(res_ptr);
	}
	else{
		*flag = 1;
		get_salt(salt, 2);
	}
	//send salt to client
	strcpy(train.buf, salt);
	train.dataLen = strlen(train.buf);
	write(socketFd, &train.flag, 4);
	write(socketFd, &train.dataLen, 4);
	write(socketFd, train.buf, train.dataLen);
	memset(&train, 0, sizeof(train));
	//receive password
	read(socketFd, &train.flag, 4);
	read(socketFd, &train.dataLen, 4);
	read(socketFd, train.buf, train.dataLen);
	strcpy(passwd, train.buf);
	memset(&train, 0, sizeof(Train_t));
	//update table or select table
	MYSQL_RES *res_ptr;
	MYSQL_ROW my_row;
	if(sign == 'i' || sign == 'I'){
		//printf("username = %s\n", user);
		//printf("password = %s\n", passwd);
		//get return of select
		sql_select(conn, "user_information", "name", user);
		res_ptr = mysql_store_result(conn);
		if(res_ptr == NULL){
			printf("mysql_store_result error\n");
			return -1;
		}
		my_row = mysql_fetch_row(res_ptr);
		if(strcmp(passwd, my_row[2]) == 0){
			//write userid
			userid = atoi(my_row[0]);
			//success
			train.flag = 22;
			write(socketFd, &train.flag, 4);
		}
		else{
			//failed
			train.flag = -1;
			write(socketFd, &train.flag, 4);
		}
	}
	else{
		//add information of new user
		sql_insert_user(conn, user, passwd, salt);
		//get userid and write it
		sql_select(conn, "user_information", "name", user);
		res_ptr = mysql_store_result(conn);
		my_row = mysql_fetch_row(res_ptr);
		userid = atoi(my_row[0]);
		train.flag = 22;
		write(socketFd, &train.flag, 4);
	}
	mysql_free_result(res_ptr);
	memset(&train, 0, sizeof(train));
	mysql_close(conn);
	return userid;
}

int get_salt(char *salt, int size){
	srand(time(NULL));
	int i;
	int flag;
	for(i = 0; i < size; ++i){
		flag = rand() % 3;
		switch(flag){
			case 0: salt[i] = rand() % 26 + 'a';
					break;
			case 1: salt[i] = rand() % 26 + 'A';
					break;
			case 2: salt[i] = rand() % 10 + '0';
					break;
		}
	}
	return 0;
}


int sql_init(MYSQL *conn, char *host, char *user, char *passwd, char *db, int port){
	mysql_init(conn);
	ERROR_CHECK(conn, NULL, "mysql_init");
	if(NULL == mysql_real_connect(conn, host, user, passwd, db, port, NULL, 0))	{
		perror("mysql_real_connect");
		return -1;
	}
	return 0;
}


int sql_insert_user(MYSQL *conn, char *user, char *passwd, char *salt){
	char query[100] = {0};
	strcat(query, "INSERT INTO ");
	strcat(query, "user_information");
	strcat(query, " (name, passwd, salt) VALUES ('");
	strcat(query, user);
	strcat(query, "', '");
	strcat(query, passwd);
	strcat(query, "', '");
	strcat(query, salt);
	strcat(query, "')");

	mysql_real_query(conn, query, strlen(query));
	printf("%s\n", query);
	return 0;
}

int sql_select(MYSQL *conn, char *tablename, char *cond, char *username){
	int flag = 0;
	if(strcmp(cond, "precode") == 0 || strcmp(cond, "userid") == 0){
		flag = 1;
	}
	char query[100] = {0};
	strcat(query, "SELECT * FROM ");
	strcat(query, tablename);
   	strcat(query, " WHERE ");
   	strcat(query, cond);
	strcat(query, " = ");
	if(flag == 0){
		strcat(query, "'");
		strcat(query, username);
		strcat(query, "'");
	}
	else{
		strcat(query, username);
	}
	printf("%s\n", query);
	mysql_query(conn, query);
	return 0;
}

int sql_insert_file(MYSQL *conn, int precode, int userid, char *filename, char *filetype, int linkcode, char *md5){
	char query[BUFSIZE] = {0};
	strcat(query, "INSERT INTO file_information (precode, userid, filename, filetype, linkcode, md5) VALUES (");
	char cprecode[10] = {0};
	sprintf(cprecode, "%d", precode);
	strcat(query, cprecode);
	strcat(query, ", ");
	char cuserid[10] = {0};
	sprintf(cuserid, "%d", userid);
	strcat(query, cuserid);
	strcat(query, ", '");
	strcat(query, filename);
	strcat(query, "', '");
	strcat(query, filetype);
	strcat(query, "', ");
	char clinkcode[10] = {0};
	sprintf(clinkcode, "%d", linkcode);
	strcat(query, clinkcode);
	strcat(query, ", ");
	if(md5 == NULL){
		strcat(query, "NULL");
	}
	else{
		strcat(query, "'");
		strcat(query, md5);
		strcat(query, "'");
	}
	strcat(query, ")");
	printf("%s\n", query);
	mysql_real_query(conn, query, strlen(query));
	return 0;
}

int	sql_insert_folder(int userid){
	MYSQL *conn;
	conn = mysql_init(NULL);
	mysql_real_connect(conn, "localhost", "root", "123456", "client", 2222, NULL, 0);
	sql_insert_file(conn, 0, userid, "dir1", "d", 0, NULL);
	sql_insert_file(conn, 0, userid, "dir2", "d", 0, NULL);
	mysql_close(conn);
	return 0;
}
