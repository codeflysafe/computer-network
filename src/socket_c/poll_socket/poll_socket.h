/*
 * @Author: sjhuang
 * @Date: 2022-03-06 11:23:46
 * @LastEditTime: 2022-03-06 11:25:20
 * @FilePath: /computer_network/src/socket_c/poll_socket/poll_socket.h
 */

#if !defined(POLL_SOCKET)
#define POLL_SOCKET
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/poll.h>
#include <math.h>
#define MAXLINE 1024
#define SERV_PORT 6666
#define BACK_LOG 100

#endif // POLL_SOCKET
