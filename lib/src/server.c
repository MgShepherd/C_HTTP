#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"
#include "constants.h"
#include "server.h"

int bind_to_socket(const struct addrinfo *con_info);
int accept_connections(int sock_fd);

H_Status h_server_start(H_Socket *server) {
  struct addrinfo *con_info;
  H_Status result = h_fill_connection_info(&con_info);
  if (result != H_SUCCESS)
    return result;

  server->sock_fd = bind_to_socket(con_info);
  freeaddrinfo(con_info);
  if (server->sock_fd == -1) {
    return H_SERVER_FAILED_TO_START;
  }

  printf("Local Server started on port %s...\n", PORT_NUMBER);

  if (accept_connections(server->sock_fd) != 0) {
    h_socket_close(server);
    return H_SERVER_FAILED_TO_START;
  }
  return result;
}

int bind_to_socket(const struct addrinfo *con_info) {
  const struct addrinfo *conn_addr = NULL;
  int sock_fd = find_valid_socket(con_info, &conn_addr);
  if (sock_fd == -1) {
    return sock_fd;
  }

  int result = bind(sock_fd, conn_addr->ai_addr, conn_addr->ai_addrlen);
  if (result == -1) {
    fprintf(stderr,
            "[ERROR] Unable to bind created socket to address, error: %s\n",
            strerror(errno));
  }

  return sock_fd;
}

int accept_connections(int sock_fd) {
  int result = listen(sock_fd, BACKLOG_SIZE);
  if (result == -1) {
    fprintf(
        stderr,
        "[ERROR] Unable to setup server to listen to connections, error: %s\n",
        strerror(errno));
    return result;
  }

  struct sockaddr_storage conn_addr;
  socklen_t conn_addr_size = sizeof(conn_addr);
  const int conn_fd =
      accept(sock_fd, (struct sockaddr *)&conn_addr, &conn_addr_size);
  if (conn_fd == -1) {
    fprintf(stderr, "[ERROR] Unable to accept connection, error: %s\n",
            strerror(errno));
  }
  printf("Accepted connection and assigned fd %d\n", conn_fd);
  return 0;
}
