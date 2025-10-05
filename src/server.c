#include "server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

static const char* PORT_NUMBER = "8080";
static const int BACKLOG_SIZE = 5;

struct M_Server {
	struct addrinfo* connect_addrs;
};

int load_connect_addrs(M_Server* server);
int create_and_bind_socket(const M_Server* server);

int m_server_start(M_Server** server) {
	*server = malloc(sizeof(M_Server));
	if (server == NULL) {
		printf("[ERROR] Unable to allocate memory required for server\n");
		return -1;
	}

	int result = load_connect_addrs(*server);
	if (result != 0) {
		return result;
	}

	const int socketfd = create_and_bind_socket(*server);
	if (socketfd < 0) {
		return socketfd;
	}

	result = listen(socketfd, BACKLOG_SIZE);	
	if (result == -1) {
		printf("[ERROR] Unable to start listening on port %s, error: %d\n", PORT_NUMBER, errno);
	}

	printf("Started server listening on port %s...\n", PORT_NUMBER);

	return 0;
}

void m_server_destroy(M_Server* server) {
	if (server == NULL) return;
	freeaddrinfo(server->connect_addrs);
	free(server);
}

int load_connect_addrs(M_Server *server) {
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int result = getaddrinfo(NULL, PORT_NUMBER, &hints, &server->connect_addrs);
	if (result != 0) {
		printf("[ERROR] Unable to retrieve address info with port: %s\n, error: %s\n", PORT_NUMBER, gai_strerror(result));
		return result;
	}
	return 0;
}

int create_and_bind_socket(const M_Server* server)  {
	if (server == NULL || server->connect_addrs == NULL) {
		printf("[ERROR] Attempted to create a socket without valid connection information\n");
		return -1;
	}

	// For Now we are just connecting to the first address returned, no need to search the linked list
	int socketfd = socket(server->connect_addrs->ai_family, server->connect_addrs->ai_socktype, server->connect_addrs->ai_protocol);
	if (socketfd == -1) {
		printf("[ERROR] Unable to create a socket for provided address, error: %d\n", errno);
		return -1;
	}

	int result = bind(socketfd, server->connect_addrs->ai_addr, server->connect_addrs->ai_addrlen);
	if (result == -1) {
		printf("[ERROR] Unable to bind socket, error: %d\n", errno);
		return -1;
	}

	return socketfd;
}

