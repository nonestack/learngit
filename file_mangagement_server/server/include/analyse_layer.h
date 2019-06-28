#ifndef __ANALYSE_LAYER_H__
#define __ANALYSE_LAYER_H__

#include "head.h"
#include "deliver_file.h"
#include "pthread_and_queue.h"

#define PATH "/home/genius/Program/file_mangagement_server/server/clond"

#define LOG_NAME "/home/genius/Program/file_mangagement_server/server/Log/log"

int analyse_order(Node_thread *);

int order_id(char *);

int exec_order(Node_thread *, int, char *, char *);

int cd_fun(Node_thread *, char *, char *);

int ls_fun(Node_thread *, char *, char *);

int puts_fun(Node_thread *, char *);

int gets_fun(Node_thread *, char *);

int remove_fun(Node_thread *, char *);

int pwd_fun(Node_thread *, char *);

int other_fun(int, char *);

#endif
