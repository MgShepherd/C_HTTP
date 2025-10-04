#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

static const char* PORT_NUMBER = "8080";

int m_connect();

int main() {
  m_connect();
  return 0;
}

int m_connect() {
  struct addrinfo *addresses;
  
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int result = getaddrinfo(NULL, PORT_NUMBER, &hints, &addresses);
  if (result != 0) {
    printf("[ERROR] Unable to retrieve address info with port: %s\n, error: %s\n", PORT_NUMBER, gai_strerror(result));
    return result;
  }
  printf("Successfully retrieved address information\n");

  struct addrinfo *current = addresses;
  char addr_str[INET6_ADDRSTRLEN];
  void *addr;
  while (current != NULL) {
    if (current->ai_family == AF_INET) {
      addr = &((struct sockaddr_in *) current->ai_addr)->sin_addr;
    } else if (current->ai_family == AF_INET6) {
      addr = &((struct sockaddr_in6 *) current->ai_addr)->sin6_addr;
    }

    const char* output = inet_ntop(current->ai_family, addr, addr_str, sizeof(addr_str));
    if (output == NULL) {
      printf("[ERROR] Unable to convert provided address into string\n");
      return -1;
    }

    printf("Found IP: %s\n", addr_str);
    
    current = current->ai_next;
  }

  freeaddrinfo(addresses);
  return 0;
}
