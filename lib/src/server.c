#include <netdb.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "server.h"

const char *PORT_NUMBER = "8080";

ServerStatus h_server_start() {
  printf("This is the server starting\n");

  struct addrinfo hints = {0};
  struct addrinfo *con_info;

  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int status = getaddrinfo(NULL, PORT_NUMBER, &hints, &con_info);
  if (status != 0) {
    fprintf(stderr, "[ERROR] Unable to retrieve address information: %s\n",
            gai_strerror(status));
    return S_FAILED_TO_START;
  }

  struct addrinfo *current = con_info;
  while (current != NULL) {
    printf("Found Address with family: %d\n", current->ai_family);
    current = current->ai_next;
  }

  freeaddrinfo(con_info);
  return S_SUCCESS;
}
