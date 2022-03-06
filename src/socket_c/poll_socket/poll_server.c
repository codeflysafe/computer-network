/*
 * @Author: sjhuang
 * @Date: 2022-03-06 11:23:19
 * @LastEditTime: 2022-03-06 11:56:52
 * @FilePath: /computer_network/src/socket_c/poll_socket/poll_server.c
 */

#include "poll_socket.h"
int main(){
    // 监听 fd 和 连接 fd
    int listenfd, coonfd, sockfd;
    int  buff[MAXLINE];
    // pollfd 列表
    struct pollfd client[OPEN_MAX];
    int maxfd1 , n, maxi = 0;
    // 进程id
    pid_t pid;
    // 
    int i, nready;
    socklen_t clilen;
    // 客户端 和 服务端 socket address
    struct sockaddr_in cliaddr, servaddr;
    // 创建 监听套接字
    listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    memset(&servaddr, 0, sizeof(servaddr));
    // 内核自己定义
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    // 绑定监听
    bind(listenfd,(struct sockaddr*)&servaddr, sizeof(servaddr));
    // 监听并设置队列长度
    listen(listenfd,BACK_LOG);   
    maxfd1 = listenfd;
    // 默认将 client[0] 作为 服务器 listenfd
    clilen[0].fd = listenfd;
    for(i = 1; i < OPEN_MAX; i++){
        client[i].fd = -1;
    }

    for(;;){
        // 
        nready = poll(pollfd, maxi + 1, -1);
        // 新来一个连接
        if((client[0].revents & POLLRDNORM)){
            clilen = sizeof(cliaddr);
            coonfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
            for(i = 0; i < OPEN_MAX; i++){
                // 存储
                if(client[i].fd < 0){
                    client[i].fd = coonfd;
                    break;
                }
            }
            // 分配完毕
            if(i == OPEN_MAX){
                printf(" to many clients\n");
                exit(1);
            }
            client[i].events = POLLRDNORM;
            if(maxi < i){
                maxi = i;
            }
            if(--nready <= 0){
                continue;
            }
        }
        
        // 遍历所有客户端，看是否可读
        for(i = 1; i < maxi; i++){
            if((client[i].fd) < 0){
                continue;
            }
            sockfd = client[i].fd;
            if(client[i].revents & (POLLRDNORM | POLLERR)){
                if((n = read(sockfd, buff, MAXLINE)) == 0){
                    // connection closed by client
                    close(sockfd);
                    client[i].fd = -1;
                }else{
                    // 传给客户端
                    write(sockfd, buff, n);
                }
                if(--nready <= 0){
                    break; // 没有可以读的 描述符了
                } 
            }

        }
    }

    return 0;
}