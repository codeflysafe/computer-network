//
// Created by sjhuang on 2022/3/15.
//

#include <unistd.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <errno.h>

int main(void)
{
    int errno;
    char buf[60];
    int epfd,nfds;
    //ev用于注册事件，数组用于返回要处理的事件
    struct epoll_event ev,events[5];
    //只需要监听一个描述符——标准输入
    epfd=epoll_create(1);
    // fd 默认的 Standard input.
    ev.data.fd = STDIN_FILENO;
    //监听读状态同时设置ET模式
    ev.events = EPOLLIN|EPOLLET;
    //注册epoll事件
    epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&ev);
    for(;;)
    {
        nfds = epoll_wait(epfd, events,5,-1);
        for(int i = 0; i< nfds;i++)
        {
            if(events[i].data.fd == STDIN_FILENO)
                read(STDIN_FILENO, buf, sizeof (buf));
                printf("Hello world!\n");
        }
    }

    return 0;
}