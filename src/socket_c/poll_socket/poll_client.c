/*
 * @Author: sjhuang
 * @Date: 2022-03-06 11:33:21
 * @LastEditTime: 2022-03-06 18:20:37
 * @FilePath: /computer_network/src/socket_c/poll_socket/poll_client.c
 */


#include "poll_socket.h"


// poll 客户端
int main(){

    // 缓冲区
    char buff[MAXLINE];
    pid_t  pid = getpid();
    int  sockfd, nready, maxfd = 1, n;
    struct sockaddr_in servaddr;
    // 创建套接字
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // 分配空间，设置为0
    memset(&servaddr, 0,  sizeof(servaddr));
    // ipv4
    servaddr.sin_family = AF_INET;
    // 设置服务器 port
    servaddr.sin_port = htons(SERV_PORT);
    // 获取 ip 地址
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // printf("%d\n", servaddr.sin_addr.s_addr);
    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if(errno != 0){
        printf("[Client] %d connection 失败 error: %d\n", pid, errno);
        exit(1);
    }
    printf("[Client] %d connection success\n", pid);
    // 标准文件
    struct pollfd fds[maxfd];
    fds[0].events = POLLRDNORM;
    fds[0].fd = sockfd;

    // 这里会阻塞住
    nready = poll(fds, maxfd, -1);
    if(nready <= 0){
        printf("[Client] %d 失败 \n", pid);
        exit(1);
    }

    // 如果可读
    if(fds[0].revents & (POLLRDNORM | ECONNRESET)){
        n = read(sockfd, buff, MAXLINE);
        if(errno != 0){
            printf("[Client] %d, error %d \n", pid, errno);
        }else if(n > 0){
            printf("[Client] %d, msg from server :%s\n", pid, buff);
        }
        fds[0].fd = -1;
    }

    close(sockfd);
    printf("[Client] %d, close \n", pid);
    return 0;
}