//
// Created by sjhuang on 2022/3/14.
//

#include "single_socket.h"

void init_serv_addr(struct sockaddr_in *serv_addr){
    serv_addr->sin_family = AF_INET;
    serv_addr->sin_port = htons(SERVER_PORT);
    serv_addr->sin_addr.s_addr = inet_addr(SERVER_IP_ADDR);
}