/*
 * @Author: sjhuang
 * @Date: 2022-03-06 11:33:21
 * @LastEditTime: 2022-03-06 18:20:37
 * @FilePath: /computer_network/src/socket_c/poll_socket/poll_client.c
 */


#include "poll_socket.h"

// fd 文件 fd
void str_cli(FILE *fd, int sockfd){
    int maxfdp1, stdineof;
    int i, n,nready;
    char buf[MAXLINE];
    int fd_num = 2;
    struct pollfd fds[fd_num];
    fds[0].fd =  fileno(fd);
    fds[0].events = POLLIN;
    fds[0].revents = 0;
    fds[1].fd =  sockfd;
    fds[1].events = POLLIN;
    fds[1].revents = 0;
    stdineof = 0;
    // 第一个是 socket 程序
    for(; ;){
        // 设置两个
        nready = poll(fds,fd_num,-1);
        // 如果可读
        if((fds[0].revents & (POLLIN | POLLRDNORM))){
            // 从 fd 中读出
            if((n = read(sockfd, buf, MAXLINE)) == 0){
               if(stdineof == 1){
                   return;
               }
            }else{
                // 写到 标准输出中
                write(fds[1].fd, buf, n);
            }

        }

        if(fds[1].revents & POLLIN){
             if((n = read(fds[1].fd, buf, MAXLINE)) == 0){
                stdineof = 1;
                // 关闭
                shutdown(sockfd, SHUT_RD);
                continue;
            }
            // 写到 标准输出中
            write(sockfd, buf, n);
        }

    }
}

// select 客户端
int main(int argc, char **argv){
    int sockfd;
    struct sockaddr_in servaddr;
    if(argc != 2){
        printf("usage: tcpcli <IPaddress>\n");
        return 0;
    }
    // 创建套接字
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // 分配空间，设置为0
    memset(&servaddr, 0,  sizeof(servaddr));
    // ipv4
    servaddr.sin_family = AF_INET;
    // 设置服务器 port
    servaddr.sin_port = htons(SERV_PORT);
    // 获取 ip 地址
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    // printf("%d\n", servaddr.sin_addr.s_addr);
    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    // stdin 代表是 shell 默认的文件
    // 标准文件
    str_cli(stdin, sockfd);

    return 0;
}