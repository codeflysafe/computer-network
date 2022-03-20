cmake_minimum_required(VERSION 3.2)

project(SINGLE_SOCKET_C)

#include(single_socket.h)

add_executable(SINGLE_SOCKET_C single_socket.c server.c)