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
  3 主事件循环(Acceptor),从事件循环(IO线程池/WORK线程池)

28:
  1 主线程中创建Connection对象，传给从事件使用
  2 如果客户端没有发送信息，那么传给IO线程使用；反之传给工作线程使用
  3 测试智能指针对线程池的影响

29:
  1 增加智能指针优化程序
  2 类的this指针并不是一个智能指针，如果此时将其传给智能指针，那么会导致两个不同
  智能指针控制块指向相同的资源，解决方法，使用std::enable_shared_from_this<T>
  3 修改当TCP对象退出时，应该删除epoll中对应的事件循环
  4 修改Channel类，Channel类封装Epoll类，用于Epoll多次循环增加、删除、修改和查询
  的事件循环
  5 修改Connection类，如果tcp断开连接则不发送数据

30:
  1 将部分裸指针替换为智能指针
  2 优化项目逻辑

31:
  1 测试eventfd
  2 测试pipe2
  3 如果计算量很小，则无需启动工作线程
  4 每个Connection对应一个客户端，管理这个客户端与服务端之间的发送/接收数据,如果
  IO线程与工作线程都存在，那么此时IO线程和工作线程都需要抢占发生缓冲区，导致竞态
  ，此时使用锁解决竞态关系并不好，因为锁的开销很大；所以可以让工作线程处理完成后
  ，交给IO线程发送

32:
  1 实现异步唤醒事件循环

33:
  1 增加TimeStamp类
  2 修改Connection类，增加时间戳，可用于清除空闲的Connection对象
  3 在事件循环中增加定时器功能，并让epoll监听定时器fd，如果达到指定事件，epoll通
  知fd的注册事件，并且进行主从循环判断，指向相对应的逻辑
 
34:
  1 事件循环分为主事件循环和从事件循环，一个事件循环可运行多个connection对象,如果
  需要管理每个时间循环的时间戳，有以下方法进行清除空闲的Connection
    1.1 在事件循环中使用map进行Connection对象的管理
    1.2 使用闹钟计时，如果时间到了，遍历map中的Connection对象，查询是否超时
    1.3 如果超时了，从map中删除Connection对象
    1.4 并且从TcpServer中删除Connection对象
  2 TcpServer在Acceptor和Connection上面一层，用于处理业务逻辑，所有管理一个
  Acceptor和多个Connection
  3 当关联容器和顺序容器进行遍历删除元素时，需要注意迭代器的失效问题，在容器中如
  果删除当前迭代器的，那么会导致这个迭代器甚至其之后的迭代器都失效

35:
  1 修改日志输出
  2 增加服务程序的退出方式

36:
  1 改善Buffer，让其支持多种格式传输
  2 测试多种拆分方式
