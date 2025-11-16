#ifndef _CLIENT_H
#define _CLIENT_H

typedef enum { C_SUCCESS, C_FAILED_TO_CONNECT } ClientStatus;

ClientStatus h_connect_to_server();

#endif // !_CLIENT_H
