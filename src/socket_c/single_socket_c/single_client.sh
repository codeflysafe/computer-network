#!/bin/zsh

## 编译
gcc -o client single_socket.c client.c -I single_socket.h
gcc -o server single_socket.c server.c -I single_socket.h

echo "编译完毕"
# 启动服务器
./server &

# 启动客户端
./client &
