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

// 服务器启动标志
static int running = 1;
// 启动一个服务器
int start_server(u_short *);
// 关闭 server
void close_server();
// 处理一个 tcp 请求
void handle_accpet(int, int);
// 读取一行数据，使用 xxx 结尾
void get_line(int, char *, int);
// 处理 request 请求
void handle_read(int, int);
// 404 not found 回复
void not_found(int);
// 错误的请求
void bad_request(int);
// 返回回复
void serve_file(int, const char*);
// 错误返回
void err_die(const char *);

#endif //EPOLL_MODE_HTTP_SERVER_H
