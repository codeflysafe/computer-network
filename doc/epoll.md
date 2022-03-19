# 彻底搞懂 epoll - ET 和 LT 模式的触发

Created: March 16, 2022 3:15 PM
Tags: epoll, io复用, tcp, 网络编程

修改和转载自，下面这一些列`blog`写的特别清晰

[彻底学会使用epoll（四）--ET的写操作实例分析-lvyilong316-ChinaUnix博客](http://blog.chinaunix.net/uid-28541347-id-4296180.html)

## LT 模式的触发方式

### 读操作

缓冲区有数据即触发

![Untitled](epoll/Untitled.png)

缓冲区有数据，下面这个程序当键盘键入数据，但是程序没有处理可读的情况。即缓冲区一直有数据，则会产生死循环。

```cpp
// src/socket_c/epoll_mode/epoll_lt_no_read.c
//
// Created by sjhuang on 2022/3/15.
//

#include <unistd.h>
#include <stdio.h>
#include <sys/epoll.h>

int main(void)
{
    int epfd,nfds;
    struct epoll_event ev,events[5];//ev用于注册事件，数组用于返回要处理的事件
    epfd = epoll_create(1);//只需要监听一个描述符——标准输入
    ev.data.fd=STDIN_FILENO;
    ev.events=EPOLLIN;//使用默认LT模式
    epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&ev);//注册epoll事件
    for(;;)
    {
        nfds=epoll_wait(epfd,events,5,-1);
        for(int i=0;i<nfds;i++)
        {
            if(events[i].data.fd == STDIN_FILENO)
            {
                printf("hello world!\n");
            }
        }
    }

    return 0;
}
```

![Untitled](epoll/Untitled%201.png)

当缓冲区的数据被读出后，在进行写入即正常，不会重复提醒

```cpp
// src/socket_c/epoll_mode/epoll_lt_read.c
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
```

![读出后正常](epoll/Untitled%202.png)

读出后正常

### 写操作

缓冲区里有空间即触发

![Untitled](epoll/Untitled%203.png)

向`buffer`写入`”hello world!”`后，虽然`buffer`没有输出清空，但是`LT`模式下只有`buffer`有写空间就返回写就绪，所以会一直输出`”hello world!”`,当`buffer`满的时候，`buffer`会自动刷清输出，同样会造成`epoll_wait`返回写就绪

```cpp
// src/socket_c/epoll_mode/epoll_lt_write.c
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
    // 监听读状态， 默认lt模式
    ev.events = EPOLLOUT;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
    for(;;){
        nready = epoll_wait(epfd, events,max_epoll_size,-1);
        for(int i =0; i < nready; i++){
            // 如果是 STDOUT_FILENO
            if(events[i].data.fd == fd){
                printf("Hello world!");
            }
        }
    }
    return 0;
}
```

![Untitled](epoll/Untitled%204.png)

## ET 模式的触发方式

### 读操作

1. 从不可读到可读
2. 有新的数据到达

![缓冲区读操作](epoll/Untitled%205.png)

缓冲区读操作

在ET模式下，缓冲区从空到有数据，但是数据一直不读取。如果没有新的数据到达，它将不再提醒。如果有新的数据到达，它便会提醒

```cpp
// src/socket_c/epoll_mode/epoll_et_no_read.c
//
// Created by sjhuang on 2022/3/16.
//

#include <unistd.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <errno.h>

int main(void)
{
    int errno;
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
                printf("Hello world!\n");
        }
    }
    return 0;
}
```

![Untitled](epoll/Untitled%206.png)

### 写操作

1. 从不可写到可写
2. 缓冲区数据变少

![缓冲区写操作](epoll/Untitled%207.png)

缓冲区写操作

```cpp
// src/socket_c/epoll_mode/epoll_et_write.c
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
```

这个程序的功能是只要标准输出写就绪，就输出`“hello world！”`。

![Untitled](epoll/Untitled%208.png)

我们发现这将是一个死循环。下面具体分析一下这个程序的执行过程：

(1) 首先初始`buffer`为空，`buffer`中有空间可写，这时无论是`ET`还是`LT`都会将对应的`epitem`加入`rdlist`，导致`epoll_wait`就返回写就绪。

(2) 程序想标准输出输出`”hello world！”`和换行符，因为标准输出为控制台的时候缓冲是“行缓冲”,所以换行符导致`buffer`中的内容清空，这就对应第二节中`ET`模式下写就绪的第二种情况——**当有旧数据被发送走时，即buffer中待写的内容变少得时候会触发fd状态的改变。**所以下次`epoll_wait`会返回写就绪。之后重复这个过程一直循环下去。

如果将 `printf("Hello world!\n");` →  `printf("Hello world!");` 则会出现下面的情况

```cpp
// src/socket_c/epoll_mode/epoll_et_write_line.c
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
               printf("Hello world!");
           }
       }
    }
    return 0;
}
```

![Untitled](epoll/Untitled%209.png)

我们看到程序成挂起状态。因为第一次`epoll_wait`返回写就绪后，程序向标准输出的`buffer`中写入`“hello world!”`，但是因为没有输出换行，所以`buffer`中的内容一直存在，下次`epoll_wait`的时候，虽然有写空间但是`ET`模式下不再返回写就绪。回忆第一节关于`ET`的实现，这种情况原因就是第一次`buffer`为空，导致`epitem`加入`rdlist`，返回一次就绪后移除此`epitem`，之后虽然`buffer`仍然可写，但是由于对应`epitem`已经不再`rdlist`中，就不会对其就绪`fd`的`events`的在检测了。

如果重新修改事件监听 `epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);`

```cpp
// src/socket_c/epoll_mode/epoll_et_write_event.c
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
               printf("Hello world!");
               epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
           }
       }
    }
    return 0;
}
```