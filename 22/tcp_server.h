/**
 * @file   tcp_server.h
 * @author Aklzzz(csdn)
 * @date   2024/03/04 19:47:13
 * @note   
 *  封装整个服务端代码
*/

#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <vector>
#include <map>

#include "acceptor.h"
#include "connection.h"
#include "my_socket.h"

class TcpServer : Noncopyable {
public:
  TcpServer(unsigned short int type, const char* ip, uint16_t port);

  ~TcpServer();

public:
  void start();

  /// @brief 处理新客户端连接请求
  void newConnection(Socket* client_sock);

  /// @brief 关闭客户端的连接
  void closeConnection(Connection* conn);

  // 客户端的连接错误
	void errorConnection(Connection* conn);

  /// @brief 处理客户端的请求报文，在Conncetion类中回调此函数
  void onMessage(Connection* conn, std::string& msg);

	// 数据发送完成后，在Conncetion类中回调此函数
	void sendComplete(Connection* conn);

	// epoll_wait()超时，在EventLoop类中回调此函数
	void epolltimeout(EventLoop* loop);

private:
  void init_(unsigned short int type, const char* ip, uint16_t port);

private:
  EventLoop loop_; // 一个TcpServer可以有多个事件循环
  Acceptor* acceptor_; // 一个TcpServer只有一个Acceptor对象
  std::map<int, Connection*> conns_;
};

#endif // TCP_SERVER_H
