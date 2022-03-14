/*
 * @Author: sjhuang
 * @Date: 2022-03-05 16:35:32
 * @LastEditTime: 2022-03-11 12:40:42
 * @FilePath: /computer_network/src/socket_c/single_socket_c/client.c
 */
#include "single_socket.h"

int main(){
    // 创建套接字
    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in serv_addr;
    // 初始化 server_addr
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDR);
    int n;
    pid_t pid = getpid();
    // 建立 tcp 连接
    int res = connect(fd, (struct sockaddr *)(&serv_addr), sizeof(serv_addr));
    if(res < 0){
        printf("[client ]connect failed, res: %d\n", res);
    }else printf("[client] connect successfully !\n");
    // 建立连接成功，传输数据到服务端
    char buf[60];
    sprintf(buf, "%d:建立连接成功\n", pid);
    n = write(fd, buf, sizeof(buf));
    if(n < 0){
        printf("[client] 发送失败 \n");
    }else printf("[client] 传输成功 \n");
    // 读取从服务器传回的数据
    // 阻塞
    n = read(fd,buf, sizeof(buf));
    if(n > 0) printf("[client] %d, Message received from server: %s\n",pid, buf);
    // 关闭套接字
    close(fd);
    printf("[client] client close: %d\n", getpid());
    return 0;
}