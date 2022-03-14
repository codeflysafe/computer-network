#！/bin/bash
###
 # @Author: sjhuang
 # @Date: 2022-03-06 15:23:19
 # @LastEditTime: 2022-03-06 15:25:24
 # @FilePath: /computer_network/src/socket_c/select_socket.c/run.sh
###

## 编译服务器和客户端程序
gcc -o server poll_server.c -I poll_socket.h
gcc -o client poll_client.c -I poll_socket.h

# 启动服务端
./server &
echo "server start!"

# 休眠一秒
sleep 2s
echo "准备启动客户端"

## 启动多个客户端
./client &
echo "client 1 start!"
./client &
echo "client 2 start!"
./client &
echo "client 3 start!"
./client &
echo "client 4 start!"
./client &
echo "client 5 start!"

#休眠一分钟
sleep 3m
pid=$(ps -ef | grep ./server | grep -v grep | awk '{print $2}')
echo "server pid is ${pid}"
# 杀掉服务端
kill -9 ${pid}
