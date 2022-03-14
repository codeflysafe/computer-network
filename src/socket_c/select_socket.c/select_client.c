/*
 * @Author: sjhuang
 * @Date: 2022-03-06 00:30:52
 * @LastEditTime: 2022-03-06 18:21:17
 * @FilePath: /computer_network/src/socket_c/select_socket.c/select_client.c
 */
#include "select_socket.h"

// select 客户端
int main(){
    char buff[MAXLINE];
    int sockfd, maxfd1, n;
    fd_set rset;
    pid_t  pid = getpid();
    // 创建套接字
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    maxfd1 = sockfd;
    struct sockaddr_in servaddr;
    // 分配空间，设置为0
    memset(&servaddr, 0,  sizeof(servaddr));
    // ipv4
    servaddr.sin_family = AF_INET;
    // 设置服务器 port
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // printf("%d\n", servaddr.sin_addr.s_addr);
    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
   if(errno != 0){
       printf("[Client] %d 建立连接失败 %d", pid, errno);
       exit(1);
   }
    printf("[Client] %d 建立连接成功 \n",pid);
    // 设置监听描述符
    FD_ZERO(&rset);
    FD_SET(sockfd,&rset);

    // 阻塞读
    select(maxfd1 + 1, &rset, NULL,NULL,NULL);
    // 如果是可读的
    if(FD_ISSET(sockfd, &rset)){
        n = read(sockfd, buff, MAXLINE);
        if(n > 0){
            printf("[client] %d msg from server: %s\n",pid, buff);
        }
        // 关闭连接
        close(sockfd);
        // 清除 sockfd 描述符监控
        FD_CLR(sockfd, &rset);
        printf("[client] close socket %d \n", pid);
    }
    return 0;
}