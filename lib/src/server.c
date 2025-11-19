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
int accept_connection(int sock_fd);
H_Status send_msg_to_client(int conn_fd);

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

  const int conn_fd = accept_connection(server->sock_fd);
  if (conn_fd == -1) {
    h_socket_close(server);
    return H_SERVER_FAILED_TO_START;
  }

  return send_msg_to_client(conn_fd);
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

int accept_connection(int sock_fd) {
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
  return conn_fd;
}

H_Status send_msg_to_client(int conn_fd) {
  static const char *MSG = "Hello from the server\n";
  const size_t msg_len = strlen(MSG);
  int bytes_sent = send(conn_fd, MSG, msg_len, 0);

  if (bytes_sent == -1) {
    fprintf(stderr, "[ERROR] Failed to send message to client, error: %s\n",
            strerror(errno));
    return H_FAILED_TO_SEND;
  } else if ((unsigned long)bytes_sent < msg_len) {
    fprintf(stderr,
            "[ERROR] Not all data was sent successfully, expected %zu but only "
            "%d sent",
            msg_len, bytes_sent);
    return H_FAILED_TO_SEND;
  }

  printf("Message successfully sent from server, number of bytes were: %d\n",
         bytes_sent);
  return H_SUCCESS;
}
