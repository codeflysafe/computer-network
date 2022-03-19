//
// Created by sjhuang on 2022/3/15.
//

#include <unistd.h>
#include <stdio.h>
#include <sys/epoll.h>

int main(void)
{
    int epfd,nfds;
    char buf[256];
    struct epoll_event ev,events[5];//ev用于注册事件，数组用于返回要处理的事件
    epfd=epoll_create(1);//只需要监听一个描述符——标准输入
    ev.data.fd = STDIN_FILENO;
    ev.events = EPOLLIN;//使用默认LT模式
    epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&ev);//注册epoll事件
    for(;;)
    {
        nfds=epoll_wait(epfd,events,5,-1);
        for(int i=0;i<nfds;i++)
        {
            if(events[i].data.fd==STDIN_FILENO)
            {
                //将缓冲中的内容读出
                read(STDIN_FILENO,buf,sizeof(buf));
                // 打印
                printf("hello world!\n");
            }
        }
    }
    return 0;
}