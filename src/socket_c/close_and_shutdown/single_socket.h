#ifndef COMPUTER_NETWORK_SINGLE_SOCKET_H
#define COMPUTER_NETWORK_SINGLE_SOCKET_H

#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

// 默认的服务器端口号
#define SERVER_PORT 1234
// 默认的服务器 ip 地址
#define SERVER_IP_ADDR "127.0.0.1"
// 套接字排队的最大连接数
// backlog = SYN_RCVD 队列长度 + ESTABLISHED 队列长度
#define BACK_LOG 12

void init_serv_addr(struct sockaddr_in *serv_addr);

#endif //COMPUTER_NETWORK_SINGLE_SOCKET_H
