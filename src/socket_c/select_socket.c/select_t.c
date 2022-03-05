/*
 * @Author: sjhuang
 * @Date: 2022-03-05 23:07:55
 * @LastEditTime: 2022-03-06 00:46:20
 * @FilePath: /computer_network/src/socket_c/select_socket.c/select_t.c
 */
#include <sys/select.h>
#include <sys/time.h>
#include <stdio.h>

int main(){
    // int, fd_set * __restrict, fd_set * __restrict,
    // fd_set * __restrict, struct timeval * __restrict
    // 
    fd_set rset;
    // 重置为0
    FD_ZERO(&rset);
    // 设置位置， 最大是1024
    FD_SET(1000, &rset);
    // 判断此位是否设置
    FD_ISSET(1000, &rset);
    // 将该位置位0
    FD_CLR(1000, &rset);

    int n = __DARWIN_howmany(__DARWIN_FD_SETSIZE, __DARWIN_NFDBITS);
    printf("len : %d, %d, %d\n",sizeof(__int32_t),__DARWIN_NFDBITS,__DARWIN_FD_SETSIZE);
    for(int i = 0; i < n; i++){
        printf("%d \n", rset.fds_bits[i]);
    }

    return 0;
}