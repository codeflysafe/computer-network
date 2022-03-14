#!/bin/zsh

## 编译
gcc -o client client.c -i single_client.h
gcc -o server server.c -i single_client.h

echo "编译完毕"
# 启动服务器
./server 1 &

# 启动客户端
./client &
