cmake_minimum_required(VERSION 3.2)

project(EPOLL_SOCKET)

include(epoll_socket.h)

add_executable(EPOLL_SOCKET epoll_server.c)