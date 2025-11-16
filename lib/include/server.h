#ifndef _SERVER_H_
#define _SERVER_H_

typedef enum { S_SUCCESS, S_FAILED_TO_START } ServerStatus;

ServerStatus server_start();

#endif // _SERVER_H_
