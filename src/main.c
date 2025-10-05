#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>

static const char* PORT_NUMBER = "8080";

int m_get_connect_addrs(struct addrinfo** addresses);
int m_display_addrs(const struct addrinfo* addresses);

int main() {
  struct addrinfo *connect_addrs;
  int result = m_get_connect_addrs(&connect_addrs);
  if (result != 0) {
    return result;
  }
  result = m_display_addrs(connect_addrs);
  if (result != 0) {
    return result;
  }

  // For Now we are just connecting to the first address returned, no need to search the linked list
  result = socket(connect_addrs->ai_family, connect_addrs->ai_socktype, 0);
  if (result == -1) {
    printf("Unable to create a socket for provided address, error: %d\n", errno);
    return result;
  }
  printf("Created a connection to localhost\n");
  
  freeaddrinfo(connect_addrs);
  
  return 0;
}

int m_get_connect_addrs(struct addrinfo** addresses) {
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int result = getaddrinfo(NULL, PORT_NUMBER, &hints, addresses);
  if (result != 0) {
    printf("[ERROR] Unable to retrieve address info with port: %s\n, error: %s\n", PORT_NUMBER, gai_strerror(result));
    return result;
  }
  return 0;
}

int m_display_addrs(const struct addrinfo* addresses) {
  const struct addrinfo *current = addresses;
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
  return 0;
}
