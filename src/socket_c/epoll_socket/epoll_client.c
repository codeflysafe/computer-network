/*
 * @Author: sjhuang
 * @Date: 2022-03-06 13:31:08
 * @LastEditTime: 2022-03-06 13:43:56
 * @FilePath: /computer_network/src/socket_c/epoll_socket/epoll_client.c
 */
#include "epoll_socket.h"
int main(){

    // epoll fd 和 socket fd
    int epfd, sockfd;
    int max_epoll_size = 5, nready, n;
    pid_t  pid = getpid();
    char buf[BUF_SIZE];
    // 创建 socket fd
    sockfd = socket(AF_INET, SOCK_STREAM ,IPPROTO_TCP);

    // 创建 server socket adder
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = ntohs(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // 创建一个 epoll fd
    epfd = epoll_create(max_epoll_size);
    // epoll event
    // ev 用于注册时间， event 用以接收准备好的时间
    struct epoll_event ev, events[max_epoll_size];
    // 注册监听事件
    ev.fd = sockfd;
    ev.events = EPOLLIN;

    // 向内核中注册事件监听
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
    nready = epoll_wait(epfd, events,max_epoll_size, -1);
    for(int i = 0; i < nready; i++){
        // 可读
        if(events[i].fd == sockfd && events[i].events & EPOLLIN){
            n = read(sockfd, buf, BUF_SIZE);
            if(n > 0){
                printf("[Client] %d received from server msg: %s\n", pid, buf);
            }
        }
    }

    // 关闭 socket 套接字
    close(sockfd);

    return 0; 
}