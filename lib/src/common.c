#include "common.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "constants.h"

H_Status h_fill_connection_info(struct addrinfo **conn_info) {
  struct addrinfo hints = {0};

  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int status = getaddrinfo(NULL, PORT_NUMBER, &hints, conn_info);
  if (status != 0) {
    fprintf(stderr, "[ERROR] Unable to retrieve address information: %s\n",
            gai_strerror(status));
    return H_FAILED_TO_LOAD_ADDRESS;
  }

  return H_SUCCESS;
}

int find_valid_socket(const struct addrinfo *conn_info,
                      const struct addrinfo **connected_addr) {
  int sock_fd = -1;
  const struct addrinfo *current;
  for (current = conn_info; current != NULL; current = current->ai_next) {
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
  }

  *connected_addr = current;
  return sock_fd;
}

void h_socket_close(H_Socket *socket) {
  if (socket->sock_fd == -1)
    return;

  int result = close(socket->sock_fd);
  if (result == -1) {
    fprintf(stderr,
            "[ERROR] Unable to close socket file descriptor, error: %s\n",
            strerror(errno));
  }
  socket->sock_fd = -1;
}
