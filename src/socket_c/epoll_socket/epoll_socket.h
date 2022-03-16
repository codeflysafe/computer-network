/*
 * @Author: sjhuang
 * @Date: 2022-03-06 13:35:13
 * @LastEditTime: 2022-03-06 14:00:26
 * @FilePath: /computer_network/src/socket_c/epoll_socket/epoll_socket.h
 */
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
#define BUF_SIZE 64


#endif // EPOLL_SOCKET
