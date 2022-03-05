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
    char buff[1024];
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
    // char buf[1024];
     // 客户端地址
    // int client_num = 0;
    struct sockaddr_in cli_addr;
    socklen_t addrlen = sizeof(cli_addr);
    for(;;){
        errno = 0;
        // 从 established 队列中获取连接，
        // 如果队列为空，则阻塞此处
        int connfd = accept(listenfd,(struct sockaddr*)&cli_addr, &addrlen);
        // 建立连接成功
        printf("[Server] connection from %s port: %d\n", inet_ntop(AF_INET, &cli_addr.sin_addr,
        buff, sizeof(buff)),ntohs(cli_addr.sin_port));

        char str[] = "hello world!\n";
        // 向客户端发送数据
        write(connfd,str, sizeof(str));
        if(errno != 0){
           printf("[Server]  connect error: %d\n", errno);
        }
        // 关闭连接
        close(connfd);
        printf(" [Server]  connect close: %d\n", connfd);
    }
    close(listenfd);
    printf("[Server]  connect close: %d\n", getpid());
    return 0;
}