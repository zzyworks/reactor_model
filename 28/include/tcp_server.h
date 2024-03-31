/**
 * @file   tcp_server.h
 * @author Aklzzz(csdn)
 * @date   2024/03/04 19:47:13
 * @note   
 *  封装整个服务端代码
*/

#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <functional>
#include <map>
#include <vector>

#include "acceptor.h"
#include "connection.h"
#include "my_socket.h"
#include "thread_pool.h"

class TcpServer : Noncopyable {
public:
  TcpServer(unsigned short int type, const char* ip, uint16_t port,
            std::size_t thread_n);

  ~TcpServer();

public:
  void start();

  /// @brief 处理新客户端连接请求
  void newConnection(Socket* client_sock);

  /// @brief 关闭客户端的连接
  void closeConnection(Connection* conn);

  /// @brief 客户端的连接错误
  void errorConnection(Connection* conn);

  /// @brief 处理客户端的请求报文，在Conncetion类中回调此函数
  void onMessage(Connection* conn, std::string& msg);

  // 数据发送完成后，在Conncetion类中回调此函数
  void sendComplete(Connection* conn);

  // epoll_wait()超时，在EventLoop类中回调此函数
  void epollTimeOut(EventLoop* loop);

  void setNewConnectionCb(std::function<void(Connection*)> fn);

  void setCloseConnectionCb(std::function<void(Connection*)> fn);

  void setErrorConnectionCb(std::function<void(Connection*)> fn);

  void setOnMessageCb(std::function<void(Connection*, std::string&)> fn);

  void setSendCompleteCb(std::function<void(Connection*)> fn);

  void setTimeOutCb(std::function<void(EventLoop*)> fn);

private:
  void init(unsigned short int type, const char* ip, uint16_t port);

private:
  EventLoop*
    main_loop_;  // 主事件循环，运行在主线程中，从事件循环运行在线程池中
  std::vector<EventLoop*> sub_loops_;  // 存放从时间循环
  ThreadPool* thread_pool_;
  std::size_t thread_n_;
  Acceptor* acceptor_;  // 一个TcpServer只有一个Acceptor对象
  std::map<int, Connection*> conns_;
  // 回调EchoServer::handleNewConnection()
  std::function<void(Connection*)> new_conn_cb_;
  // 回调EchoServer::handleClose()
  std::function<void(Connection*)> close_conn_cb_;
  // 回调EchoServer::handleError()
  std::function<void(Connection*)> error_conn_cb_;
  // 回调EchoServer::handleMessage()
  std::function<void(Connection*, std::string&)> on_msg_cb_;
  // 回调EchoServer::handleSendComplete()
  std::function<void(Connection*)> send_compl_cb_;
  // 回调EchoServer::handleTimeOut()
  std::function<void(EventLoop*)> time_out_cb_;
};

#endif  // TCP_SERVER_H
