# TCP 协议与套接字编程

Created: March 5, 2022 3:09 PM
Tags: socket, tcp, 网络编程

> 代码在 [[github]](https://github.com/codeflysafe/computer-network/tree/main/src/socket_c/single_socket_c)
>

## `TCP` 协议

---

`TCP` 是运输层的协议，它提供客户与服务器之间的连接。

它提供了一下功能：

可靠性、流量控制、自动重传

`TCP` 传输的是**报文段**

![tcp 首部](TCP%20%E5%8D%8F%E8%AE%AE%E4%B8%8E%E5%A5%97%E6%8E%A5%E5%AD%97%2087d22/Untitled.png)

tcp 首部

## **套接字地址结构**

---

大多数套接字函数都需要一个指向套接字地址结构的指针作为参数。每个协议族都定义他自己的套接字地址结构。这些结构均以 `sockaddr_` 开头，并以对应每个协议族的唯一后缀。

### `IPv4` **套接字地址结构**

![tcp 套接字结构](TCP%20%E5%8D%8F%E8%AE%AE%E4%B8%8E%E5%A5%97%E6%8E%A5%E5%AD%97%2087d22/Untitled%201.png)

tcp 套接字结构

```cpp
/*
 * Internet address (a structure for historical reasons)
 */
struct in_addr {
	in_addr_t s_addr; // 32位 ipv4地址
};

/*
 * Socket address, internet style.
 */
struct sockaddr_in {
	__uint8_t       sin_len; // 长度
	sa_family_t     sin_family; // 协议类型 AF_INET 代表 ipv4
	in_port_t       sin_port;  // 端口号 16位
	struct  in_addr sin_addr; // ip 地址 32位
	char            sin_zero[8]; // 不知道干啥的
  // sin_addr + sin_zero 刚好 12 个字节
  // 共有 16个字节
};
```

### 通用套接字地址结构

---

内核用来存储大多是地址的套接字地址结构, 唯一用途是对指向特定协议的套接字地址结构的指针执行强制类型转换

```cpp
/*
 * [XSI] Structure used by kernel to store most addresses.
 */
struct sockaddr {
	__uint8_t       sa_len;         /* total length */
	sa_family_t     sa_family;      /* [XSI] address family */
	char            sa_data[14];    /* [XSI] addr value (actually larger) */
  // 共有 16个字节
};
```

```cpp
struct sockaddr_in serv_addr;
bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
```

## `TCP` 基本套接字

---

`tcp` 建立流程

![tcp 流程图](TCP%20%E5%8D%8F%E8%AE%AE%E4%B8%8E%E5%A5%97%E6%8E%A5%E5%AD%97%2087d22/Untitled%202.png)

tcp 流程图

结合着`TCP`的三次握手、数据传输以及四次挥手来学习

### `socket` 函数

为了执行网络i/o, 一个进程首先做的是调用 `socket` 函数，指定通行协议类型。

```cpp
int fd = socket(int family, int type, int protocol);
```

| 字段 | 说明 | 取值 |
| --- | --- | --- |
| family | 协议族（ip） | AF_INET(ipv4), AF_INET6(ipv6) |
| type | 套接字类型，与 family / protocol组合使用 | SOCK_STREAM(TCP) SOCK_DGRAM(UDP) |
| protocol | 运输层协议 | IPPROTO_TCP/IPPROTO_UDP |

### TCP 建立连接（三次握手）

涉及到的函数有 `socket`

![tcp 三次握手与tcp套接字函数的关系](TCP%20%E5%8D%8F%E8%AE%AE%E4%B8%8E%E5%A5%97%E6%8E%A5%E5%AD%97%2087d22/Untitled%203.png)

tcp 三次握手与tcp套接字函数的关系

主要涉及函数为 `connect`、`bind`、`listen`、`accept`

![tcpdump 的结果](TCP%20%E5%8D%8F%E8%AE%AE%E4%B8%8E%E5%A5%97%E6%8E%A5%E5%AD%97%2087d22/Untitled%204.png)

tcpdump 的结果

### `connect` **函数**

客户端调用`connect` 函数来建立与`TCP`服务器端的连接， `tcp`客户端调用`connect`， 会发送一个报文段（第一个握手）给服务器端。此时客户端的状态发生改变：`closed` → `syn_sent`， 当`connect` 返回后， `socket`的状态便变为了`established`状态

```cpp
int connect(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen);
// Return 0 on success, -1 for errors.
```

`connect` 会激发`TCP`的三次握手，仅在连接建立成功或者出错时才返回， 出错的情况如下：

|  | errno | 描述 | 错误原因 |
| --- | --- | --- | --- |
| ECONNREFUSED | 61 | Connection refused | PORT 没有在监听 |
| ETIMEDOUT | 60 | Operation timed out | 没有收到 SYN 分节的响应 |
| ENETUNREACH | 51 | Network is unreachable | ICMP 目的地不可达错误 |

**`ECONNREFUSED` `tcpdum` 结果**

```cpp
22:20:56.815703 IP localhost.62252 > localhost.search-agent: Flags [S], seq 632562553, win 65535, options [mss 16344,nop,wscale 6,nop,nop,TS val 2517767049 ecr 0,sackOK,eol], length 0
22:20:56.815741 IP localhost.search-agent > localhost.62252: Flags [R.], seq 0, ack 632562554, win 0, length 0
```

### `bind`  **函数**

`bind`函数将本地地址协议赋予一个套接字 `socket fd`。通常是 `32`位的 `ipv4`地址 + `tcp` 端口或者`udp`端口。

```cpp
int  bind(int socket_fd, const struct sockaddr *server_addr, socklen_t addrlen);
```

`server_addr` 见上文

调用`bind`是，可以指定`port`或者`ip`，也可以不指定（不指定的话内核会自动分配一个`port`和`ip`）。服务器端通常指定 `port`，而客户端通常选用内核自动分配

执行 `single_socket.sh` 脚本，可以看到下面结果（**把对应注释去掉**）

指定客户端端口号（`client bind`）

```cpp
single_socket_c git:(main) ✗ ./single_client.sh
编译完毕
[client] 41243 connect successfully !
[Server] connection from 127.0.0.1 port: 6666
[client] 41243, Message received from server: hello world!

[client] client close: 41243
server pid is 41242
```

内核分配端口号

```cpp
single_socket_c git:(main) ✗ ./single_client.sh
编译完毕
[Server] connection from 127.0.0.1 port: 63017
[client] 41609 connect successfully !
[client] 41609, Message received from server: hello world!

[client] client close: 41609
server pid is 41608
```

### `listen` **函数**

```cpp
int __sys_listen(int fd, int backlog)
{
	struct socket *sock;
	int err, fput_needed;
	int somaxconn;

	sock = sockfd_lookup_light(fd, &err, &fput_needed);
	if (sock) {
		somaxconn = sock_net(sock->sk)->core.sysctl_somaxconn;
		if ((unsigned int)backlog > somaxconn)
			backlog = somaxconn;

		err = security_socket_listen(sock, backlog);
		if (!err)
			err = sock->ops->listen(sock, backlog);

		fput_light(sock->file, fput_needed);
	}
	return err;
}
```

`listen` 函数一般是服务器端在 `socket` `bind` 之后调用， 它主要功能有两个：

1. 将`socket`的状态变为被动打开（`closed  → listen`）
2. `backlog` 设置相应套接字队列排队的最大连接个数 `（min(backlog, somaxconn)`

```cpp
int  listen(int sock_fd, int backlog);
```

![backlog 和 套接字排队队列](TCP%20%E5%8D%8F%E8%AE%AE%E4%B8%8E%E5%A5%97%E6%8E%A5%E5%AD%97%2087d22/Untitled%205.png)

backlog 和 套接字排队队列

未完成连接队列： `server` 接收到了来自 `client` 的 `SYN` 请求，正在等待完成三次握手过程。此时这些`socket` 处于 `syn_rcvd` 状态

已完成连接队列： 每个已完成`tcp`三路握手过程的客户端对应其中的一项，这些`socket`处于 `established` 状态

### `accept` 函数

`server`调用 `accept` 函数，会从就绪队列拿出一个`socket` 请求，然后创建一个`socket`对象，并返回其`fd`（成为`connect fd` )，此时三次握手正式完成，`socket`状态变为`established`。

```cpp
int accept(int sock_fd, struct sockaddr * __restrict, socklen_t * __restrict)
```

此时，若就绪队列（完成连接队列）为空，则会阻塞直到其存在完成连接。

### TCP 数据传输

### TCP 断开连接（四次挥手）

**###** `close`**函数**