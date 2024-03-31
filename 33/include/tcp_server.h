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
  using ConnectionPtr = Connection::ConnectionPtr;
  using NewConnCb = std::function<void(ConnectionPtr)>;
  using CloseConnCb = std::function<void(ConnectionPtr)>;
  using ErrorConnCb = std::function<void(ConnectionPtr)>;
  using OnMsgCb = std::function<void(ConnectionPtr, std::string&)>;
  using SendComplCb = std::function<void(ConnectionPtr)>;

  TcpServer(unsigned short int type, const char* ip, uint16_t port,
            std::size_t thread_n);

  ~TcpServer() = default;

  void start();

  /// @brief 处理新客户端连接请求
  void newConnection(std::unique_ptr<Socket> client_sock);

  /// @brief 关闭客户端的连接
  void closeConnection(ConnectionPtr conn);

  /// @brief 客户端的连接错误
  void errorConnection(ConnectionPtr conn);

  /// @brief 处理客户端的请求报文，在Conncetion类中回调此函数
  void onMessage(ConnectionPtr conn, std::string& msg);

  // 数据发送完成后，在Conncetion类中回调此函数
  void sendComplete(ConnectionPtr conn);

  // epoll_wait()超时，在EventLoop类中回调此函数
  void epollTimeOut(EventLoop* loop);

  void setNewConnectionCb(NewConnCb fn);

  void setCloseConnectionCb(CloseConnCb fn);

  void setErrorConnectionCb(ErrorConnCb fn);

  void setOnMessageCb(OnMsgCb fn);

  void setSendCompleteCb(SendComplCb fn);

  void setTimeOutCb(std::function<void(EventLoop*)> fn);

private:
  void init(unsigned short int type, const char* ip, uint16_t port);

  std::unique_ptr<EventLoop>
    main_loop_;  // 主事件循环，运行在主线程中，从事件循环运行在线程池中
  std::vector<std::unique_ptr<EventLoop>> sub_loops_;  // 存放从时间循环
  std::size_t thread_n_;                               // 线程池大小
  std::unique_ptr<ThreadPool> thread_pool_;  // IO线程池,发送/接收tcp缓存的内容
  std::unique_ptr<Acceptor> acceptor_;  // 一个TcpServer只有一个Acceptor对象
  std::map<int, ConnectionPtr> conns_;
  // 回调EchoServer::handleNewConnection()
  NewConnCb new_conn_cb_;
  // 回调EchoServer::handleClose()
  CloseConnCb close_conn_cb_;
  // 回调EchoServer::handleError()
  ErrorConnCb error_conn_cb_;
  // 回调EchoServer::handleMessage()
  OnMsgCb on_msg_cb_;
  // 回调EchoServer::handleSendComplete()
  SendComplCb send_compl_cb_;
  // 回调EchoServer::handleTimeOut()
  std::function<void(EventLoop*)> time_out_cb_;
};

#endif  // TCP_SERVER_H
