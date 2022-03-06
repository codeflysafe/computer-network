/*
 * @Author: sjhuang
 * @Date: 2022-03-06 00:30:07
 * @LastEditTime: 2022-03-06 11:20:37
 * @FilePath: /computer_network/src/socket_c/select_socket.c/select_server.c
 */

#include "select_socket.h"
int main(){
    // 监听 fd 和 连接 fd
    int listenfd, coonfd, sockfd;
    // 描述符集合
    fd_set rset, allset;
    int client[FD_SETSIZE], buff[MAXLINE];
    int maxfd1 , n, maxi = -1;
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
    // 最大是1024
    for(i = 0; i < FD_SETSIZE; i++){
        client[i] = -1;
    }
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);
    for(;;){
        rset = allset;
        // 
        nready = select(maxfd1 + 1, &rset, NULL, NULL, NULL);
        // 新来一个连接
        if((FD_ISSET(listenfd, &rset))){
            clilen = sizeof(cliaddr);
            coonfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
            for(i = 0; i < FD_SETSIZE; i++){
                // 存储
                if(client[i] < 0){
                    client[i] = coonfd;
                    break;
                }
            }
            // 分配完毕
            if(i == FD_SETSIZE){
                printf(" to many clients\n");
                exit(1);
            }
            // 增加新的描述符
            FD_SET(coonfd, &rset);
            // 如果比最大的maxfd1还要大，
            // 则更新 maxfd
            if(maxfd1 < coonfd){
                maxfd1 = coonfd;
            }
            if(maxi < i){
                maxi = i;
            }
            if(--nready <= 0){
                continue;
            }
        }
        
        // 遍历所有客户端，看是否可读
        for(i = 0; i < maxi; i++){
            if((sockfd = client[i]) < 0){
                continue;
            }
            if(FD_ISSET(sockfd, &rset)){
                if((n = read(sockfd, buff, MAXLINE)) == 0){
                    // connection closed by client
                    close(sockfd);
                    FD_CLR(sockfd, &rset);
                    client[i] = -1;
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