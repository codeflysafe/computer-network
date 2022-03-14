#!/bin/bash

## 编译
gcc client.c -o client
gcc multi_pro_server.c -o multi_pro_server

## 启动服务器
./multi_pro_server &
echo "服务器已启动"

## 启动三个客户端
./client &
echo "客户端1已启动"
./client &
echo "客户端2已启动"
./client &
echo "客户端3已启动"

pid=$(ps -ef | grep ./server | grep -v grep | awk '{print $2}')
echo "server pid is ${pid}"
# 杀掉服务端
kill -9 ${pid}