# Kabi
- 可实现一键搭载高性能RPC服务器

kabi采用的是主从Reactor模型， 有一个mainReactor和多个subReactor。
mainReactor由主线程运行：通过epoll监听listenfd可读事件，当可读事件发生后，accept一个clientfd，然后随机读取一个subReactor，将clientfd的读写事件注册到subReactor的epoll上即可。
也就是说，mainReactor只负责建立连接事件，不进行业务处理，也不关心已连接套接字的IO事件

采用应用层buffer
方便数据处理，特别是应用层的包组装和拆解
方便异步的发送（发送数据直接塞到发送缓冲里，等待epoll异步去发送
提高发送效率，多个包合并到一起发送
TcpBuffer封装
Inbuffer：服务端调用read成功从socket缓冲区读到数据，会写入Inbuffer后面
服务端从Inbuffer前面读取数据，进行解码得到请求
Outbuffer：服务端向外发送数据，会将数据编码后写入到outbuffer后面
服务端在fd可写的情况下，调用write将outbuffer里面的数据全部发送出去


TcpConnection模块封装，循环执行Read、Excute、Write
Read：读取客户端发来的数据，组装为RPC请求
Excute：将RPC请求作为入参， 执行业务逻辑得到RPC响应
Write：将RPC响应发送给客户端

TcpClient模块封装，设置为非阻塞，
Connect：连接对端机器
Write：将RPC响应发送给客户端
Read：读取客户端发来的数据，组装为RPC请求
返回0, 表示连接成功
返回-1,errno==EINPROGRESS 表示正在建立连接，此时可以添加到epoll中去监听其可写事件，等待可写事件就绪后，调用getsockopt获取fd上的错误，错误为0代表连接建立成功
其他errno报错
