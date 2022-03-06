/*
 * @Author: sjhuang
 * @Date: 2022-03-06 11:33:21
 * @LastEditTime: 2022-03-06 11:41:24
 * @FilePath: /computer_network/src/socket_c/poll_socket/poll_client.c
 */


#include "poll_socket.h"

// fd 文件 fd
void str_cli(FILE *fd, int sockfd){
    int maxfdp1, stdineof;
    int n;
    // 读取 fd_set
    fd_set rset;
    char buf[MAXLINE];
    stdineof = 0;
    FD_ZERO(&rset);
    for(; ;){
        if(stdineof == 0){
            // 监听 file fd
            FD_SET(fileno(fd), &rset);
        }
        FD_SET(sockfd, &rset);
        if(sockfd > fileno(fd)){
            maxfdp1 = sockfd;
        }else {
            maxfdp1 = fileno(fd);
        }
        // maxfdp1 = max(fileno(fd), sockfd) + 1;
        // time = NULL 代表采用 阻塞方式，只有准备好了之后才返回
        select(maxfdp1, &rset, NULL, NULL, NULL);
        
        // 如果网络 readble
        if(FD_ISSET(sockfd, &rset)){
            // 这里不会在等待数据准备，
            // 直接从内核缓冲区复制到用户进程
            if((n = read(sockfd, buf, MAXLINE))){
                // 是否含有 EOF 符号
                if(stdineof == 1){
                    return;
                }else{

                }
            }
            // 写入文件中
            write(fileno(fd), buf, n);
        }

        // 如果文件准备好了
        if(FD_ISSET(fileno(fd), &rset)){
            // 读取完毕
            if(( n= read(fileno(fd), buf, MAXLINE) == 0)){
                stdineof = 1;
                // shut down the reading side
                // send fin
                shutdown(sockfd, SHUT_RD);
                // 重置 监听描述符
                FD_CLR(fileno(fd), &rset);
                continue;
            }
            // 向网络中传输
            write(sockfd, buf, n);
        }
    }
}

// select 客户端
int main(int argc, char **argv){
    int sockfd;
    struct sockaddr_in servaddr;
    if(argc != 2){
        printf("usage: tcpcli <IPaddress>\n");
        return 0;
    }
    // 创建套接字
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // 分配空间，设置为0
    memset(&servaddr, 0,  sizeof(servaddr));
    // ipv4
    servaddr.sin_family = AF_INET;
    // 设置服务器 port
    servaddr.sin_port = htons(SERV_PORT);
    // 获取 ip 地址
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    // printf("%d\n", servaddr.sin_addr.s_addr);
    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    // stdin 代表是 shell 默认的文件
    // 标准文件
    str_cli(stdin, sockfd);

    return 0;
}