#！/bin/bash
###
 # @Author: sjhuang
 # @Date: 2022-03-06 15:23:19
 # @LastEditTime: 2022-03-06 15:25:24
 # @FilePath: /computer_network/src/socket_c/select_socket.c/run.sh
### 

## 编译服务器和客户端程序
gcc -o server select_server.c -I sekect_socket.h
echo "server gcc finish!"
# 
gcc -o client select_client.c -I sekect_socket.h
echo "client gcc finish!"

# 启动服务端
./server
echo "server start!"

## 启动多个客户端
./server
echo "server start!"