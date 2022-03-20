//
// Created by sjhuang on 2022/3/20.
//

#ifndef EPOLL_MODE_HTTP_SERVER_H
#define EPOLL_MODE_HTTP_SERVER_H

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
#define SERVER_IP "127.0.0.1"
#define BUF_SIZE 1024*1024
#define BACK_LOG 100
#define MAX_EPOLL_SIZE 1024

#endif //EPOLL_MODE_HTTP_SERVER_H
