#include "client.h"
#include <stdlib.h>

int main() {
	M_Client* client = NULL;
	int result = m_client_connect(&client);
	if (result != 0) {
		return result;
	}
  
	m_client_destroy(client);
	return 0;
}
