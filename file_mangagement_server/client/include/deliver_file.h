#ifndef __DELIVER_FILE_H__
#define __DELIVER_FILE_H__

#include "head.h"

int send_file(int, char *);

int recv_file(int, char *);

int send_cycle(int, void *, int);

int recv_cycle(int, void *, int);

int send_md5(int, char *);
#endif
