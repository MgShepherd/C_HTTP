#ifndef _COMMON_H_
#define _COMMON_H_

#include "status.h"

#include <netdb.h>

typedef struct {
  int sock_fd;
} H_Socket;

H_Status h_fill_connection_info(struct addrinfo **conn_info);
int find_valid_socket(const struct addrinfo *conn_info,
                      const struct addrinfo **connected_addr);

void h_socket_close(H_Socket *socket);

#endif // !_COMMON_H_
