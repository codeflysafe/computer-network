# socket编程中write、read和send、recv之间的区别
> from https://www.cnblogs.com/George1994/p/6731091.html

read/write
ssize_t read(int fd,void *buf,size_t nbyte)
ssize_t write(int fd, const void*buf,size_t nbytes);

read：负责从描述符fd中读取内容，当读取成功时，read返回实际所读的字节数（大于0）；如果返回的值为0，表示已经读到文件结束了；如果返回的值小于0，则出现错误，如果错误为EINTR，说明错误是由中断引起的，如果是ECONNREST表示网络连接出了问题。

write：将buf中的nbytes字节内容写入到文件描述符fd，成功时返回写的字节数，失败的时候返回-1；在实际的程序中，写入有两种可能：

write的返回值大于0,表示写了部分或者是全部的数据。这样我们用一个while循环来不停的写入，但是循环过程中的buf参数和nbyte参数得由我们来更新。也就是说，网络写函数是不负责将全部数据写完之后在返回的。
返回的值小于0,此时出现了错误。我们要根据错误类型来处理.如果错误为EINTR表示在写的时候出现了中断错误。如果为EPIPE表示网络连接出现了问题(对方已经关闭了连接).
recv/send
int recv(int sockfd,void *buf,int len,int flags)
int send(int sockfd,void *buf,int len,int flags)

第四个参数可以是0或者是下面的组合

flags	描述
MSG_DONTROUTE	不查找表
MSG_OOB	接受或者发送带外数据
MSG_PEEK	查看数据,并不从系统缓冲区移走数据
MSG_WAITALL	等待所有数据
读取／接收	写入／发送
read	write
recv	send
当recv/send的flag参数设置为0时，则和read/write是一样的。
如果有如下几种需求，则read/write无法满足，必须使用recv/send:

为接收和发送进行一些选项设置
从多个客户端中接收报文
发送带外数据（out-of-band data)
参考文章：
socket编程中write、read和send、recv之间的区别