#include "server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static const char* PORT_NUMBER = "8080";
static const int BACKLOG_SIZE = 5;

static const char* EXAMPLE_MSG = "Hello World!\n";

struct M_Server {
	struct addrinfo* connect_addrs;
	int socket_fd;
};

void get_string_address(const struct addrinfo* address, char* output, size_t max_len);
int load_connect_addrs(M_Server* server);
int create_and_bind_socket(const M_Server* server);
int accept_connection(const M_Server* server);

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

	(*server)->socket_fd = create_and_bind_socket(*server);
	if ((*server)->socket_fd < 0) {
		return (*server)->socket_fd;
	}

	result = listen((*server)->socket_fd, BACKLOG_SIZE);
	if (result == -1) {
		printf("[ERROR] Unable to start listening on port %s, error: %d\n", PORT_NUMBER, errno);
	}

	char connection_address[INET6_ADDRSTRLEN];
	get_string_address((*server)->connect_addrs, connection_address, INET6_ADDRSTRLEN);
	printf("Started server %s listening on port %s..\n", connection_address, PORT_NUMBER);

	result = accept_connection(*server);

	return 0;
}

void m_server_destroy(M_Server* server) {
	if (server == NULL) return;
	freeaddrinfo(server->connect_addrs);
	free(server);
}

void get_string_address(const struct addrinfo* address, char* output, size_t max_len) {
	void* addr;
	if (address->ai_family == AF_INET) {
		addr = &((struct sockaddr_in *)address->ai_addr)->sin_addr;
	} else if (address->ai_family == AF_INET6) {
		addr = &((struct sockaddr_in6 *)address->ai_addr)->sin6_addr;
	} else {
		return;
	}

	inet_ntop(address->ai_family, addr, output, max_len);
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

int accept_connection(const M_Server *server) {
	if (server == NULL || server->socket_fd < 0) {
		printf("[ERROR] Unable to use existing socket to accept incoming requests\n");
		return -1;
	}

	struct sockaddr connected_addr;
	socklen_t addr_len = sizeof(connected_addr);
	int connect_fd = accept(server->socket_fd, &connected_addr, &addr_len);
	if (connect_fd == -1) {
		printf("[ERROR] Unable to accept incoming connection, error: %d\n", errno);
		return -1;
	}

	int result = send(connect_fd, EXAMPLE_MSG, strlen(EXAMPLE_MSG), 0);
	if (result < 0) {
		printf("[ERROR] Unable to send message to connected client, error: %d\n", errno);
		return result;
	}

	close(connect_fd);

	return 0;
}
