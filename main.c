//
// Created by sjhuang on 2022/3/16.
//

#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
int main(){
    // 32 位
    int a;
    // 64 为系统， 应该是 8
    int *b;
    char *buf = "123132";
    char msg[10] = "123132";
    char c = '1';
    // 6 + 1？
    char mm[] = "123132";
    struct sockaddr addr;
    struct sockaddr_in sockaddrIn;



    printf("%d \n", sizeof(a));
    printf("%d \n", sizeof(b));
    printf("%d \n", sizeof(buf));
    printf("%d \n", sizeof(msg));
    printf("%d \n", sizeof(c));
    printf("%d \n", sizeof(mm));
    printf("%d \n", sizeof(addr));
    printf("%d \n", sizeof(sockaddrIn));
    return 0;
};