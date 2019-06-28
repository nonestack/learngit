#ifndef __ANALYSE_LAYER_H__
#define __ANALYSE_LAYER_H__

#include "head.h"
#include "deliver_file.h"

int analyse_order(int);

int order_id(char *);

int exec_order(int, int, char *, char *);

int cd_fun(int, char *, char *);

int ls_fun(int, char *, char *);

int puts_fun(int, char *);

int gets_fun(int, char *);

int remove_fun(int, char *);

int pwd_fun(int, char *);

int other_fun(int, char *);

#endif
