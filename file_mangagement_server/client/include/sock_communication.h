#ifndef __SOCK_COMMUNICATION_H__
#define __SOCK_COMMUNICATION_H__

#include "head.h"

int sock_communication(int);

int deal_train(int, Train_t *, char *);

int receive_cd(Train_t *, char *);

int receive_ls(Train_t *);

int receive_puts(int, char *);

int receive_gets(int, Train_t *);

int receive_remove(Train_t *);

int receive_pwd(Train_t *);

int receive_other(Train_t *);
#endif
