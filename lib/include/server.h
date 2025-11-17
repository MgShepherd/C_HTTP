#ifndef _SERVER_H_
#define _SERVER_H_

#include "status.h"

typedef struct {
  int sock_fd;
} H_Server;

H_Status h_server_start(H_Server *server);
void h_server_stop(H_Server *server);

#endif // _SERVER_H_
