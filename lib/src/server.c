#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "server.h"
#include "utils.h"
#include "constants.h"

int bind_to_socket(const struct addrinfo *con_info);
int accept_connections(int sock_fd);

H_Status h_server_start(H_Server *server) {
  struct addrinfo *con_info;
	H_Status result = h_fill_connection_info(&con_info);
	if(result != H_SUCCESS) return result;

  server->sock_fd = bind_to_socket(con_info);
  if (server->sock_fd == -1) {
    h_server_stop(server);
    return H_SERVER_FAILED_TO_START;
  }

  freeaddrinfo(con_info);
  printf("Local Server started on port %s...\n", PORT_NUMBER);

  if (accept_connections(server->sock_fd) != 0) {
    h_server_stop(server);
    return H_SERVER_FAILED_TO_START;
  }
  return result;
}

void h_server_stop(H_Server *server) {
  if (server->sock_fd == -1)
    return;

  int result = close(server->sock_fd);
  if (result == -1) {
    fprintf(stderr,
            "[ERROR] Unable to close socket file descriptor, error: %s\n",
            strerror(errno));
  }
  server->sock_fd = -1;
}

int bind_to_socket(const struct addrinfo *con_info) {
  const struct addrinfo *current;
  int sock_fd = -1;
  for (current = con_info; current != NULL; current = current->ai_next) {
    sock_fd =
        socket(current->ai_family, current->ai_socktype, current->ai_protocol);
    if (sock_fd != -1)
      break;
  }

  if (sock_fd == -1) {
    fprintf(stderr,
            "[ERROR] Unable to open socket, last attempt to open socket failed "
            "with error: %s\n",
            strerror(errno));
    return sock_fd;
  }

  int result = bind(sock_fd, current->ai_addr, current->ai_addrlen);
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
