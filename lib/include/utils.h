#ifndef _UTILS_H_
#define _UTILS_H_

#include "status.h"

#include <netdb.h>

typedef struct {
  int sock_fd;
} H_Socket;

H_Status h_fill_connection_info(struct addrinfo **connect_addr);

void h_socket_close(H_Socket *socket);

#endif // !_UTILS_H_
