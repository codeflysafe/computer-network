//
// Created by sjhuang on 2022/3/20.
//

#include "http_server.h"


// 
void disconnect(int connfd, int epfd){
    int ret = epoll_ctl(epfd, EPOLL_CTL_DEL, connfd, NULL);
    if(ret == -1){
        err_die("disconnect error");
    }
    // close connected
    close(connfd);
}

void get_line(int client, char *buf, int size){
    int i = 0;
    char c = '\0';
    int n = 0;
    // read line
    while((i < size - 1) && (c != '\n')){
        // read char
        n = recv(client, &c, 1, 0);
        if(n > 0){
            if(c == '\r'){
                n = recv(client, &c, 1, MSG_PEEK);
                // '\r\n' 
                if(c == '\n'){
                    // 
                    recv(client, &c, 1, 0);
                }
            }else{
                buf[i] = c;
                c = '\n';
            }
        }
        i++;
    }
    buf[i] = '\0';
}

void send_response_header(int client, int status, const char *desc, const char *type, long len){
    char buf[512] = {0};
    sprintf(buf,"HTTP/1.1 %d %s\r\n",status,desc);
    send(client, buf, strlen(buf), 0);
    sprintf(buf,"Content-type: %s\r\n",type);
    send(client, buf, strlen(buf), 0);
    sprintf(buf,"Content-Length: %ld\r\n",len);
    send(client, buf, strlen(buf), 0);
    send(client, "\r\n", 2, 0);
}

void bad_request(int client)
{

    send_response_header(client, 400, "BAD REQUEST", "text/html", 30);
    /*回应客户端错误的 HTTP 请求 */
    char content[] = "<p>This is an simple response </p>";
    send(client, content, strlen(content), 0);
   
}

void send_msg(int client){

    /*回应客户端的 HTTP 请求 */
    // response header
    send_response_header(client,200, "OK", "text/html", 30);
    char content[] = "<p>This is an simple response </p>";
    send(client, content, strlen(content), 0);
}

void send_file(int client,  const char* file_name){
    
}

// 处理连接操作
void handle_accpet(int sockfd, int epfd){
    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    errno = 0;
    char buf[1024] = {0};
    // 获取连接
    int coonfd = accept(sockfd,(struct sockaddr *)&cli_addr, &cli_len);
    if(coonfd <= 0){
        err_die("Server socket accept error");
    }else{
        printf("[Server] establish from %s port: %d\n", inet_ntop(AF_INET, &cli_addr.sin_addr,
                                                                   buf, sizeof(buf)),ntohs(cli_addr.sin_port));
    }
    // 监听可读请求
    struct epoll_event ev;
    ev.data.fd = coonfd;
    ev.events = EPOLLIN ;
    // 注册监听事件
    epoll_ctl(epfd, EPOLL_CTL_ADD, coonfd, &ev);
}

void handle_read(int coonfd, int epfd){
    errno = 0;
    int n = 0, nbytes = 0;
    char buf[BUF_SIZE] = {0};
    // 如果可读，就将缓冲区读完
    n = read(coonfd, buf, sizeof(buf));
    if(n <= 0){
        err_die("Server read msg error");
    }

    // 获取读取的数据
    printf("Sever get msg from client: %s\n", buf);
    // parser http request

    // bad_request(coonfd);
    // 处理读取的数据
    // todo 处理读取的数据
    send_msg(coonfd);
    disconnect(coonfd,epfd);
    // todo 回复客户端 
    // 去除监听的事件
    // struct epoll_event ev;
    // ev.data.fd = coonfd;
    // ev.events = EPOLLIN;
    // // 删除监听事件
    // epoll_ctl(epfd, EPOLL_CTL_DEL, coonfd, &ev);

    // 关闭 连接
    // shutdown(coonfd, SHUT_WR);
}

void err_die(const char *err_info){
    perror(err_info);
    exit(1);
}

int server_socket(u_short *port){
    // socket fd connect fd epoll fd
    int sockfd, epfd;
    errno = 0;
    int n, nready;
    // 地址协议
    struct sockaddr_in ser_addr;
    // 初始化 服务器端 地址
    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_port = ntohs(*port);
    ser_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    ser_addr.sin_family = AF_INET;

    // 创建连接套接字
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sockfd <= 0){
        err_die("Server socket create error");
    }else printf("Server socket create success: %d\n",sockfd);
    // 监听
    n = bind(sockfd, (struct sockaddr *)&ser_addr, sizeof(ser_addr));
    if(n == -1){
        err_die("Server socket bind error");
    }
    // 被动打开
    listen(sockfd, BACK_LOG);
    printf("Server socket bind port:%s: %d\n",SERVER_IP, SERVER_PORT);
    socklen_t add_len = sizeof(ser_addr);
    // 未设定 监听的 port
    if(*port == 0){
        if(getsockname(sockfd, (struct sockaddr *)&ser_addr, &add_len) == -1){
            err_die("get server port error");
        }
        *port = ntohs(ser_addr.sin_port);
    }
    return sockfd;
}

void start_server(int sockfd){
    int n, nready;
    // 监听 sock fd 套接字
    struct  epoll_event ev, events[MAX_EPOLL_SIZE];
    ev.data.fd = sockfd;
    ev.events = EPOLLIN;

    // 创建 epoll fd
    int epfd = epoll_create(MAX_EPOLL_SIZE);
    // 监听事件 sock fd 的可读事件
    epoll_ctl(epfd,EPOLL_CTL_ADD, sockfd,&ev);
    for(;;){
        printf("Server 正在监听链接\n");
        // 阻塞获取事件
        nready = epoll_wait(epfd, events, MAX_EPOLL_SIZE, -1);
        for(int i = 0; i < nready; i++){
            // 如果是新的请求连接到达，处理它
            if(events[i].data.fd == sockfd){
                handle_accpet(sockfd, epfd);
            }else if(events[i].events & EPOLLIN){
                // 处理可读请求
                handle_read(events[i].data.fd, epfd);
            }
        }
    }

}

// 一种 http 请求服务器. 采用`epoll i/o`复用机制
int main(){
    u_short port = SERVER_PORT;
    // 地址协议
    int sockfd = server_socket(&port);
    start_server(sockfd);
    return 0;
}