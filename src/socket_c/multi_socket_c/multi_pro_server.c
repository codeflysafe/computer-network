#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(){
    int listenfd, connfd;
    char buff[60];
    // pid
    pid_t pid;
    // 创建套接字，获取 listenfd
    listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // 
    struct sockaddr_in serv_addr;
    // 填充 0
    memset(&serv_addr, 0, sizeof(serv_addr));
    // ipv4
    serv_addr.sin_family = AF_INET;
    // 端口号
    serv_addr.sin_port = htons(1234);
    // 具体的ip地址
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // 监听， 将服务器协议地址赋予给一个套接字
    bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
    // 套接字排队的最大连接数
    // backlog = SYN_RCVD 队列长度 + ESTABLISHED 队列长度
    int backlog = 10;
    // 监听 listenfd, 将获取的 SYN 的连接放入 SYN_RCVD 队列中
    listen(listenfd, backlog);
     // 客户端地址
    struct sockaddr_in cli_addr;
    socklen_t addrlen = sizeof(cli_addr);
    for(;;){
         errno = 0;
        // 从 established 队列中获取连接，
        // 如果队列为空，则阻塞此处
        int connfd = accept(listenfd,(struct sockaddr*)&cli_addr, &addrlen);
        // 如果是子进程
        if((pid = fork()) == 0){
            // 关键监听fd
            close(listenfd);
           // 建立连接成功
            printf("[Server] connection from %s port: %d\n", inet_ntop(AF_INET, &cli_addr.sin_addr,
            buff, sizeof(buff)),ntohs(cli_addr.sin_port));
            char str[] = "hello world!\n";
           // 向客户端发送数据
            write(connfd,str, sizeof(str));
           // 关闭连接
           close(connfd);
           printf(" [Server]  connect close: %d\n", c);
           exit(0);
        }
        close(connfd);
    }
    // 关闭套接字
    close(listenfd);
    printf("[Server]  connect close: %d\n", getpid());
    return 0;
}