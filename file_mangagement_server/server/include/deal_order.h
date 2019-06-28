#ifndef __DEAL_ORDER_H__
#define __DEAL_ORDER_H__

#include "head.h"

#define VISITOR_PATH "/home/genius/Program/file_mangagement_server/server/clond"

int sock_communication(int);

int explain_order(char *, char *);

int deal_order(int, char *, int, char *);

int cd_fun(char *, int, char *);

int ls_fun(char *, int, char *);

int puts_fun(char *, int, char *);

int gets_fun(char *, int, char *);

int remove_fun(char *, int, char *);

int pwd_fun(char *, int, char *);

int other_fun(int);

#endif
