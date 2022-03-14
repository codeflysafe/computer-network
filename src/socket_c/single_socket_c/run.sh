#!/bin/bash

## 编译
gcc -o client client.c -i single_client.h
gcc -o server server.c -i single_client.h

## 启动服务器
./server &
echo "服务器已启动"

## 启动三个客户端
./client &
echo "客户端1已启动"
./client &
echo "客户端2已启动"
./client &
echo "客户端3已启动"