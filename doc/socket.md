<!--
 * @Author: sjhuang
 * @Date: 2022-03-05 20:31:18
 * @LastEditTime: 2022-03-11 09:36:39
 * @FilePath: /computer_network/doc/socket.md
-->
# TCP 套接字函数

## 套接字地址结构
大多数套接字函数都需要一个指向套接字地址结构的指针作为参数。每个协议族都定义他自己的套接字地址结构。这些结构均以`sockaddr_` 开头，并以对应每个协议族的唯一后缀。

### `IPv4` 套接字地址结构

```c++
/*
 * Internet address (a structure for historical reasons)
 */
struct in_addr {
	in_addr_t s_addr;
};
```

### `socket`函数
```c++
int fd = socket(int family, int type, int protocol);
```
主要作用是创建 `socket fd`

### `connect`函数


### `bind`函数

### `listen`函数



### `close`函数

### `select` 函数