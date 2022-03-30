#!/bin/bash

## 编译服务器和客户端程序
gcc -o server http_server.c

# 启动服务端
./server 
echo "server start!"

