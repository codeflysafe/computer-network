//
// Created by sjhuang on 2022/3/20.
//

#include "http_server.h"

// 处理连接操作
void handle_accpet(int sockfd, int epfd, struct  sockaddr_in *cli_addr, socklen_t *cl_len){
    errno = 0;
    char buf[1024] = {0};
    // 获取连接
    int coonfd = accept(sockfd,(struct sockaddr *)cli_addr, cl_len);
    if(coonfd <= 0){
        printf("Server socket accept error: %d\n",errno);
        exit(1);
    }else{
        printf("[Server] establish from %s port: %d\n", inet_ntop(AF_INET, &cli_addr->sin_addr,
                                                                   buf, sizeof(buf)),ntohs(cli_addr->sin_port));
    }
    // 监听可读请求
    struct epoll_event ev;
    ev.data.fd = coonfd;
    ev.events = EPOLLIN;
    // 注册监听事件
    epoll_ctl(epfd, EPOLL_CTL_ADD, coonfd, &ev);
}

void handle_read(int coonfd, int epfd){
    errno = 0;
    int n = 0, nbytes = 0;
    char buf[BUF_SIZE] = {0};
    // 如果可读，就将缓冲区读完
    n = read(coonfd, buf, sizeof(buf));
    // 获取读取的数据
    if(n > 0){
        printf("Sever get msg from client: %s\n", buf);
    }else{
        printf("Server read msg error \n");
    }
    // 去除监听的事件
    struct epoll_event ev;
    ev.data.fd = coonfd;
    ev.events = EPOLLIN;
    // 删除监听事件
    epoll_ctl(epfd, EPOLL_CTL_DEL, coonfd, &ev);

    // 处理读取的数据
    // todo 处理读取的数据
    const char msg[] = "HTTP/1.1 200 OK \r\n \r\n Hello world!";
    // 回复客户端数据
    write(coonfd, msg, sizeof (msg));
    // todo 回复客户端

    // 关闭 连接
    close(coonfd);
}

// 一种 http 请求服务器. 采用`epoll i/o`复用机制
int main(){
    // socket fd connect fd epoll fd
    int sockfd, epfd;
    errno = 0;
    int n, nready;
    // 地址协议
    struct sockaddr_in ser_addr, cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    // 初始化 服务器端 地址
    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_port = ntohs(SERVER_PORT);
    ser_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    ser_addr.sin_family = AF_INET;

    // 创建连接套接字
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sockfd <= 0){
        printf("Server socket create error: %d\n",errno);
        exit(1);
    }
    printf("Server socket create success: %d\n",sockfd);
    // 监听
    n = bind(sockfd, (struct sockaddr *)&ser_addr, sizeof(ser_addr));
    if(n == -1){
        printf("Server socket bind error: %d\n", errno);
        exit(1);
    }
    // 被动打开
    listen(sockfd, BACK_LOG);
    printf("Server socket bind port:%s: %d\n",SERVER_IP, SERVER_PORT);

    // 监听 sock fd 套接字
    struct  epoll_event ev, events[MAX_EPOLL_SIZE];
    ev.data.fd = sockfd;
    ev.events = EPOLLIN;

    // 创建 epoll fd
    epfd = epoll_create(MAX_EPOLL_SIZE);
    // 监听事件 sock fd 的可读事件
    epoll_ctl(epfd,EPOLL_CTL_ADD, sockfd,&ev);

    for(;;){
        printf("Server 正在监听链接\n");
        // 阻塞获取事件
        nready = epoll_wait(epfd, events, MAX_EPOLL_SIZE, -1);
        for(int i = 0; i < nready; i++){
            // 如果是新的请求连接到达，处理它
            if(events[i].data.fd == sockfd){
                handle_accpet(sockfd, epfd, &cli_addr, &cli_len);
            }else if(events[i].events & EPOLLIN){
                // 处理可读请求
                handle_read(events[i].data.fd, epfd);
            }
        }
    }

    return 0;
}