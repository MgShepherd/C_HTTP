#include "client.h"
#include "status.h"
#include "utils.h"

#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

H_Status h_client_connect(H_Socket *client) {
  struct addrinfo *conn_addr;
  H_Status result = h_fill_connection_info(&conn_addr);
  if (result != H_SUCCESS) {
    return result;
  }

  client->sock_fd = socket(conn_addr->ai_family, conn_addr->ai_socktype,
                           conn_addr->ai_protocol);
  if (client->sock_fd == -1) {
    return H_CLIENT_FAILED_TO_CONNECT;
  }

  if (connect(client->sock_fd, conn_addr->ai_addr, conn_addr->ai_addrlen) ==
      -1) {
    fprintf(stderr,
            "[ERROR] Unable to connect to server, connection failed "
            "with error: %s\n",
            strerror(errno));
    h_socket_close(client);
    return H_CLIENT_FAILED_TO_CONNECT;
  }

  printf("Client successfully connected to server\n");

  return H_SUCCESS;
}
