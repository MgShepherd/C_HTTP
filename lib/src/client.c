#include "client.h"
#include "common.h"
#include "status.h"

#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

H_Status h_client_connect(H_Socket *client) {
  struct addrinfo *conn_info;
  H_Status result = h_fill_connection_info(&conn_info);
  if (result != H_SUCCESS) {
    return result;
  }

  const struct addrinfo *conn_addr = NULL;
  client->sock_fd = find_valid_socket(conn_info, &conn_addr);
  if (client->sock_fd == -1) {
    freeaddrinfo(conn_info);
    return H_CLIENT_FAILED_TO_CONNECT;
  }

  if (connect(client->sock_fd, conn_addr->ai_addr, conn_addr->ai_addrlen) ==
      -1) {
    fprintf(stderr,
            "[ERROR] Unable to connect to server, connection failed "
            "with error: %s\n",
            strerror(errno));
    freeaddrinfo(conn_info);
    h_socket_close(client);
    return H_CLIENT_FAILED_TO_CONNECT;
  }

  printf("Client successfully connected to server\n");
  freeaddrinfo(conn_info);

  return H_SUCCESS;
}
