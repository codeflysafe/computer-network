/*
 * @Author: sjhuang
 * @Date: 2022-03-06 00:30:52
 * @LastEditTime: 2022-03-06 00:46:28
 * @FilePath: /computer_network/src/socket_c/select_socket.c/select_client.c
 */

#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/select.h>

// fd 文件 fd
void str_cli(FILE *fd, int sockfd){
    int maxfdp1, stdineof;
    // 读取 fd_set
    fd_set rset;
}

// select 客户端
int main(){
    // 创建套接字
    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    return 0;
}