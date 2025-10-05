#include "client.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

static const char* PORT_NUMBER = "8080";

struct M_Client {
	struct addrinfo* connect_addrs;
};

int load_connect_addrs(M_Client* client);
int create_socket(const M_Client* client);

int m_client_connect(M_Client** client) {
	*client = malloc(sizeof(M_Client));
	if (client == NULL) {
		printf("[ERROR] Unable to allocate memory required for client\n");
		return -1;
	}

	int result = load_connect_addrs(*client);
	if (result != 0) {
		return result;
	}

	result = create_socket(*client);
	if (result != 0) {
		return result;
	}

	return 0;
}

void m_client_destroy(M_Client* client) {
	if (client == NULL) return;
	freeaddrinfo(client->connect_addrs);
	free(client);
}

int load_connect_addrs(M_Client *client) {
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int result = getaddrinfo(NULL, PORT_NUMBER, &hints, &client->connect_addrs);
	if (result != 0) {
		printf("[ERROR] Unable to retrieve address info with port: %s\n, error: %s\n", PORT_NUMBER, gai_strerror(result));
		return result;
	}
	return 0;
}

int create_socket(const M_Client* client)  {
	if (client == NULL || client->connect_addrs == NULL) {
		printf("[ERROR] Attempted to create a socket without valid connection information\n");
		return -1;
	}

	// For Now we are just connecting to the first address returned, no need to search the linked list
	int socket = socket(client->connect_addrs->ai_family, client->connect_addrs->ai_socktype, client->connect_addrs->ai_protocol);
	if (socket == -1) {
		printf("[ERROR] Unable to create a socket for provided address, error: %d\n", errno);
		return -1;
	}

	printf("Created a socket for connecting to localhost\n");
	return 0;
}

