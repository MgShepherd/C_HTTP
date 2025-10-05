#ifndef _M_SERVER_H_
#define _M_SERVER_H_

typedef struct M_Server M_Server;

int m_server_start(M_Server** server);
void m_server_destroy(M_Server* server);

#endif // _M_SERVER_H_
