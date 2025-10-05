#include "server.h"
#include <stdlib.h>

int main() {
	M_Server* server = NULL;
	int result = m_server_start(&server);
	if (result != 0) {
		return result;
	}
  
	m_server_destroy(server);
	return 0;
}
