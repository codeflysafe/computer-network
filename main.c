//
// Created by sjhuang on 2022/3/16.
//

#include <stdio.h>

int main(){
    // 32 位
    int a;
    int *b;
    char *buf = "123132";
    char msg[10] = "123132";
    char c = '1';
    char mm[] = "123132";

    printf("%d \n", sizeof(a));
    printf("%d \n", sizeof(b));
    printf("%d \n", sizeof(buf));
    printf("%d \n", sizeof(msg));
    printf("%d \n", sizeof(c));
    printf("%d \n", sizeof(mm));
    return 0;
};