#include "client.h"
#include "common.h"
#include "status.h"

#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#define MAX_MSG_LEN 100

H_Status read_msg_from_server(int conn_fd);

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

  return read_msg_from_server(client->sock_fd);
}

H_Status read_msg_from_server(int conn_fd) {
  char msg_buffer[MAX_MSG_LEN];
  int bytes_read = recv(conn_fd, msg_buffer, MAX_MSG_LEN, 0);

  if (bytes_read == -1) {
    fprintf(stderr, "[ERROR] Failed to read message from server, error: %s\n",
            strerror(errno));
    return H_FAILED_TO_RECV;
  } else if (bytes_read == 0) {
    fprintf(stderr, "[ERROR] Failed to read from server due to connection "
                    "already being closed");
    return H_FAILED_TO_RECV;
  }

  printf("Successfully recieved message: %s\n", msg_buffer);
  return H_SUCCESS;
}
