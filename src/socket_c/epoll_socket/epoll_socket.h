#if !defined(EPOLL_SOCKET)
#define EPOLL_SOCKET
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>

#define SERVER_PORT 8888
#define SERVER_IP "127.0.0.01"
#define BUF_SIZE 1024
#define BACK_LOG 100


#endif // EPOLL_SOCKET
