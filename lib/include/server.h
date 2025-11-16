#ifndef _SERVER_H_
#define _SERVER_H_

typedef enum { S_SUCCESS, S_FAILED_TO_START } ServerStatus;

typedef struct {
  int sock_fd;
} Server;

ServerStatus h_server_start(Server *server);
void h_server_stop(Server *server);

#endif // _SERVER_H_
