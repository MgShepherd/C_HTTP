#ifndef _UTILS_H_
#define _UTILS_H_

#include "status.h"

#include <netdb.h>

H_Status h_fill_connection_info(struct addrinfo **connect_addr);

#endif // !_UTILS_H_
