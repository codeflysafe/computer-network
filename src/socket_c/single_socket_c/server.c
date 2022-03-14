/*
 * @Author: sjhuang
 * @Date: 2022-03-05 16:35:28
 * @LastEditTime: 2022-03-11 12:44:02
 * @FilePath: /computer_network/src/socket_c/single_socket_c/server.c
 */
#include "single_socket.h"
int main(int argc, char **argv){

    if(argc != 2){
        printf("Must has two argument \n");
        exit(1);
    }
    // 获取要连接的个数
    int client_nums = atoi(argv[1]);
    printf("[Server] 要链接的数量 %d \n", client_nums);
    // listenfd 监听 fd
    // connfd 连接 fd
    int listenfd, connfd, n;
    char buff[30];
    // 创建套接字，获取 listenfd
    listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // 常见 sockaddr 结构
    struct sockaddr_in serv_addr;
    // 填充 0
    memset(&serv_addr, 0, sizeof(serv_addr));
    // ipv4
    serv_addr.sin_family = AF_INET;
    // 端口号
    serv_addr.sin_port = htons(SERVER_PORT);
    // 具体的ip地址
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDR);
    // 监听， 将服务器协议地址赋予给一个套接字
    bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));

    // 监听 listenfd, 将获取的 SYN 的连接放入 SYN_RCVD 队列中
    listen(listenfd, BACK_LOG);
    // char buf[1024];
     // 客户端地址
    // int client_num = 0;
    struct sockaddr_in cli_addr;
    socklen_t addrlen = sizeof(cli_addr);
    int i = 0;
    for(; ;){
        errno = 0;
        // 从 established 队列中获取连接，
        // 如果队列为空，则阻塞此处
        connfd = accept(listenfd,(struct sockaddr*)&cli_addr, &addrlen);
        if(connfd < 0){
            printf("[Server] 获取连接失败 \n");
            break;
        } else {
            // printf("[Server] connection from %s port: %d\n", inet_ntop(AF_INET, &cli_addr.sin_addr,buff, sizeof(buff)),ntohs(cli_addr.sin_port));
            // 建立连接成功
            printf("[Server] 获取连接成功\n");
        }
        n = read(connfd, buff, sizeof(buff));
        // 读取数据
        if(n > 0) printf("[Server] recevied: %s \n", buff);
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