#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(){
    char buffer[40];
    pid_t  pid = getpid();
    // 创建套接字
    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(1234);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    errno = 0;
    // 建立 tcp 连接
    int res = connect(fd, (struct sockaddr *)(&serv_addr), sizeof(serv_addr));
    if(res != 0){
        printf(" [Client] %d connect error: %d\n", pid, errno);
    }
    // 读取从服务器传回的数据
    read(fd,buffer, sizeof(buffer)-1);
    printf("[Client] %d Message from server: %s\n", pid, buffer);
    // 关闭套接字
    close(fd);
}