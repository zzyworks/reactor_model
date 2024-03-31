/***********************************************************
  > File Name: echo_server.h
  > Author: Aklzzz
  > Mail: 2429605732@qq.com
  > Created Time: Sat 09 Mar 2024 03:52:41 PM CST
  > Modified Time:Sat 09 Mar 2024 03:52:41 PM CST
 *******************************************************/

// 回显服务器

#ifndef ECHO_SERVER_H
#define ECHO_SERVER_H

#include "tcp_server.h"
#include "thread_pool.h"

class EchoServer : Noncopyable {
public:
  using ConnectionPtr = TcpServer::ConnectionPtr;

  EchoServer(unsigned short int type, const char* ip, uint16_t port,
             std::size_t sub_thread_n = 3, std::size_t work_thread_n = 5);

  ~EchoServer() = default;

  // 启动服务
  void start();

  // 处理新客户端连接请求，在TcpServer类中回调此函数
  void handleNewConnection(ConnectionPtr conn);

  // 关闭客户端的连接，在TcpServer类中回调此函数
  void handleClose(ConnectionPtr conn);

  // 客户端的连接错误，在TcpServer中回调此函数
  void handleError(ConnectionPtr conn);

  // 处理客户端的请求报文，在TcpServer中回调此函数
  void handleMessage(ConnectionPtr conn, std::string& msg);

  // 数据发送完成后，在TcpServer中回调此函数
  void handleSendComplete(ConnectionPtr conn);

  // epoll_wait()超时，在TcpServer中回调此函数
  void handleTimeOut(EventLoop* loop);

  // 处理客户端的请求报文，用于添加给线程池
  void onMessage(ConnectionPtr conn, std::string& msg);

private:
  void init();

  TcpServer tcp_serv_;
  std::unique_ptr<ThreadPool> thread_pool_;  // 工作线程池
};

#endif  // ECHO_SERVER_H
