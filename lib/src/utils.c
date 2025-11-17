#include "utils.h"

#include <stddef.h>
#include <stdio.h>

#include "constants.h"

H_Status h_fill_connection_info(struct addrinfo **connect_addr) {
  struct addrinfo hints = {0};

  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int status = getaddrinfo(NULL, PORT_NUMBER, &hints, connect_addr);
  if (status != 0) {
    fprintf(stderr, "[ERROR] Unable to retrieve address information: %s\n",
            gai_strerror(status));
    return H_FAILED_TO_LOAD_ADDRESS;
  }

  return H_SUCCESS;
}
