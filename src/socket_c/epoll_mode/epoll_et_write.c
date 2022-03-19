//
// Created by sjhuang on 2022/3/19.
//

#include <stdlib.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <unistd.h>
int main(){
    int fd, epfd;
    int max_epoll_size = 5, nready;
    epfd = epoll_create(max_epoll_size);
    struct epoll_event ev,events[max_epoll_size];
    // tandard output
    fd = STDOUT_FILENO;
    ev.data.fd = fd;
    // 监听读状态同时设置ET模式
    ev.events = EPOLLOUT | EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
    for(;;){
       nready = epoll_wait(epfd, events,max_epoll_size,-1);
       for(int i =0; i < nready; i++){
           // 如果是 STDOUT_FILENO
           if(events[i].data.fd == fd){
               printf("Hello world!\n");
           }
       }
    }
    return 0;
}