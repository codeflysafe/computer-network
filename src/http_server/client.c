/*
 * @Author: sjhuang
 * @Date: 2022-03-05 16:35:32
 * @LastEditTime: 2022-03-11 12:40:42
 * @FilePath: /computer_network/src/socket_c/single_socket_c/client.c
 */
#include "http_server.h"

int main(){
    errno = 0;
    char buf[60];
    // 创建套接字
    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in serv_addr;
    // 指定端口号，
    // struct sockaddr_in cli_addr;
    // 初始化 cli_addr
//    memset(&cli_addr, 0, sizeof(serv_addr));
//    // 指定端口
//    cli_addr.sin_port = htons(6666);
//    bind(fd, (struct sockaddr *)(&cli_addr), sizeof(cli_addr));

    // 初始化 server_addr
    memset(&serv_addr, 0, sizeof(serv_addr));
    init_serv_addr(&serv_addr);
    int n;
    pid_t pid = getpid();
    // 建立 tcp 连接
    int res = connect(fd, (struct sockaddr *)(&serv_addr), sizeof(serv_addr));
    if(res < 0){
        printf("[client ] connect failed, res: %d, errno: %d\n", res, errno);
    }else printf("[client] %d connect successfully !\n", pid);
    // 读取从服务器传回的数据
    // 阻塞
    n = read(fd,buf, sizeof(buf));
    if(n > 0) printf("[client] %d, Message received from server: %s\n",pid, buf);
    // 关闭套接字
    close(fd);
    printf("[client] client close: %d\n", getpid());
    return 0;
}