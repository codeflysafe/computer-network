/*
 * @Author: sjhuang
 * @Date: 2022-03-06 13:31:17
 * @LastEditTime: 2022-03-06 13:44:00
 * @FilePath: /computer_network/src/socket_c/epoll_socket/epoll_serve.c
 */
#include "epoll_socket.h"

int main(){

    int n, nready;
    // 描述符 fd
    int sockfd, connfd, epfd, fd;
    int max_epoll_size = 100;
    char buf[BUF_SIZE];
    // 创建 server 和 client address
    // server 用来监听
    // client 用来获得 client 信息
    struct sockaddr_in ser_addr, cli_addr;
    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_port = ntohs(SERVER_PORT);
    ser_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    ser_addr.sin_family = AF_INET;

    socklen_t cli_len = sizeof(cli_addr);

    // 创建 socket fd
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // 监听 端口
    bind(sockfd,(struct sockaddr *)&ser_addr, sizeof(ser_addr));
    // 将服务器状态修改为 listen，并设置就绪队列长度
    listen(sockfd,BACK_LOG);

    struct epoll_event ev,events[max_epoll_size];
    epfd = epoll_create(max_epoll_size);

    ev.data.fd = sockfd;
    ev.events = EPOLLIN;

    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

    char *msg = "Hello World!";
    for(;;){
        nready = epoll_wait(epfd, events, max_epoll_size, -1);
        for(int i = 0; i < nready; i++){
            if(events[i].data.fd == sockfd && events[i].events & EPOLLIN){
                // 收到client 发来的建立连接请求
                connfd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_len);
                if(connfd > 0){
                    printf("[Server] connection from %s port: %d\n", inet_ntop(AF_INET, &cli_addr.sin_addr,
                                                                               buf, sizeof(buf)),ntohs(cli_addr.sin_port));
                    // 连接建立完成，发送消息给客户端
                    write(connfd, msg, 1000);
                    // 向 epoll 注册新的监听事件
                    struct epoll_event conn_ev;
                    conn_ev.data.fd = connfd;
                    conn_ev.events = EPOLLIN;
                    epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &conn_ev);
                }else break;
            }else if(events[i].data.fd > 0 && events[i].events & EPOLLIN){
                // 收到 client 发来的消息
                connfd = events[i].data.fd;
                n = read(connfd, buf, BUF_SIZE);
                // 接收成功
                if(n > 0){
                    printf("[Server] received msg from client: %s \n", buf);
                }else {
                    printf("[Server] close connect client: %d \n", connfd);
                    // 删掉该 fd 的监控
                    epoll_ctl(epfd, EPOLL_CTL_DEL, connfd, NULL);
                    // 错误或者是没有数据，关闭连接
                    close(connfd);
                }
            }
        }
    }

    // 关闭 socket fd
    close(sockfd);

}