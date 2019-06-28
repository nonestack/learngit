#ifndef __DELIVER_FILE_H__
#define __DELIVER_FILE_H__

#include "head.h"
#include "pthread_and_queue.h"

int send_file(Node_thread *, char *);

int recv_file(Node_thread *, char *, char *);

int send_cycle(int, void *, int);

int recv_cycle(int, void *, int);

int recv_md5(Node_thread *, char *, char *);

#endif
