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
    char  buff[MAXLINE];
    // pollfd 列表
    struct pollfd client[OPEN_MAX];
    int maxfd1 , n, maxi = 0;
    // 进程id
    pid_t pid;
    // 
    int i, nready;
    // 客户端 和 服务端 socket address
    struct sockaddr_in cli_addr, servaddr;
    socklen_t clilen = sizeof(cli_addr);
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
    // 默认将 client[0] 作为 服务器 listenfd
    client[0].fd = listenfd;
    client[0].events = POLLRDNORM;
    for(i = 1; i < OPEN_MAX; i++){
        client[i].fd = -1;
    }
    char *msg = "Hello world!\n";
    for(;;){
        // 
        nready = poll(client, OPEN_MAX, -1);
        // 新来一个连接
        if((client[0].revents & POLLRDNORM)){
            coonfd = accept(listenfd, (struct sockaddr*)&cli_addr, &clilen);
            if(errno != 0){
                printf("[Server] connect error %d\n", errno);
                break;
            }
            printf("[Server] connection from %s port: %d\n", inet_ntop(AF_INET, &cli_addr.sin_addr,
                                                                       buff, sizeof(buff)),ntohs(cli_addr.sin_port));
            write(coonfd, msg, 1000);
            for(i = 1; i < OPEN_MAX; i++){
                // 存储
                if(client[i].fd < 0){
                    client[i].fd = coonfd;
                    client[i].events = POLLRDNORM;
                    break;
                }
            }
            // 分配完毕
            if(i == OPEN_MAX){
                printf(" to many clients\n");
                exit(1);
            }
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
                if((n = read(sockfd, buff, MAXLINE)) < 0){
                    // connection closed by client
                   if(errno == ECONNRESET){
                       close(sockfd);
                       printf("[server] close socket %d %d \n", sockfd, errno);
                       client[i].fd = -1;
                   }
                }else if(n == 0){
                    close(sockfd);
                    printf("[server] close socket %d \n", sockfd);
                    client[i].fd = -1;
                }else {
                    printf("[server] receive msg %s \n", buff);
                }
                if(--nready <= 0){
                    break; // 没有可以读的 描述符了
                } 
            }

        }
    }

    return 0;
}