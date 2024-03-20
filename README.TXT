文件夹1：
  1 创建了一个简单的epoll客户端与服务端

文件夹2：
  优化文件夹1的项目：
    1 int socket(int domain, int type, int protocol)，在type填写SOCK_NONBLOCK，直接创建非阻塞的socket
    2 int accept4(int sockfd, struct sockaddr *addr, socklen_t *addrlen, int flags)，在flags填写SOCK_NONBLOCK，直接创建非阻塞的socket

文件夹3：
  1 添加InetAddress类
  2 添加Noncopyable类

文件夹4：
  1 添加Socket类

文件夹5：
  1 添加Epoll类

文件夹6：
  1 添加Channel类
  2 修改Epoll类

文件夹1：
  1 测试添加Channel类

文件夹7：
  1 修改Channel类

文件夹8：
  1 添加回调函数
    1.1 什么是回调函数？
      1.1.1 函数的名字可以做参数
      1.1.2 那个做参数的函数叫回调函数
    1.2 回调函数的作用
      1.2.1 恰当的时机，发送通知
  2 回调函数用来解决什么问题？
    2.1 主线程执行过程中需要调用dbFunc()，调用之后主线程还需要知道这个调用返回的结果，并处理这个结果，此时可以将处理这个结果的内容封装为一个函数callback()，然后让这个函数作为参数一并传给dbFunc(callback)，此时callback被称为回调函数
    2.2 这样做回调函数可以解放主线程，主线程就相当于"不需要在乎执行后的结果了"，因为回调函数会将其做完
    2.3 回调函数可以将异步"转为"同步

文件夹9：
  1 添加EventLoop类

文件夹11：
  1 添加TcpServer类

文件夹12：
  1 添加Acceptor类

文件夹13:
  1 添加Connection类
  2 修改Channel类

文件夹14：
  1 修改TcpServer类
  2 修改Connection类

文件夹14：
  1 修改Channel类
  2 修改Acceptor类

文件夹15：
  1 修改TcpServer类
  2 修改Acceptor类

文件夹16：
  1 修改TcpServer类
  2 修改Acceptor类
  3 修改Socket类
  4 修改Connection类
  5 修改InetAddress类

文件夹17:
  1 修改Connection类
  2 修改Channel类
  3 修改Acceptor类

文件夹18：
  1 修改TcpServer类
  2 修改Connection类

文件夹19_0：TCP的粘包与分包
  1 demo文件夹用于测试TCP的粘包与分包
  2 如何解决TCP的粘包与分包问题？
    2.1 demo1文件夹，采用固定长度的报文
    2.2 demo2文件夹，在报文前面加上报文长度；报文头部（4字节的整数）+ 报文内容

文件夹19_1：
  1 增加Buffer类
  2 修改Connection类
  3 修改Channel类
  4 修改Socket类
  5 修改Acceptor类

20：
  1 使用接收缓冲区input_buffer

21:
  1 使用发送缓冲区output_buffer  
  2 send()将数据放入TCP的发送缓冲区，不是Connection的发送缓冲区

22:
	1 再次优化回调函数
	2 让服务端进程知道数据发送完毕
	3 设置超时服务

23:
	1 TcpServer类为底层类封装的接口
	2 新添加的EchoServer类为业务类，如果不需要某个业务，则无需调用TcpServer的接口

23_2:
	1 修改影响性能的行为，例如值传递

24:
	1 修改Buffer类
	2 修改Connection类
	3 修改EchoServer类

25:
	1 增加了线程池ThreadPool类
	2 测试ThreadPool类

26:
  1 添加主从reactor模型，主模型用于监听，从模型用于处理客户端新连接
  2 从模型运行于线程池

27:
  1 增加工作线程，用于处理计算型任务
  2 修改IO线程，用于处理buffer发送/接收tcp缓冲
