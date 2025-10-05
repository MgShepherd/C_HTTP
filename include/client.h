#ifndef _M_CLIENT_H_
#define _M_CLIENT_H_

typedef struct M_Client M_Client;

int m_client_connect(M_Client** client);
void m_client_destroy(M_Client* client);

#endif // _M_CLIENT_H_
