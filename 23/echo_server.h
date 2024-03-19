/***********************************************************
  > File Name: echo_server.h
  > Author: Aklzzz
  > Mail: 2429605732@qq.com
  > Created Time: Sat 09 Mar 2024 11:10:45 AM CST
  > Modified Time:Sat 09 Mar 2024 11:10:45 AM CST
 *******************************************************/

// 回显服务器

#ifndef ECHO_SERVER_H
#define ECHO_SERVER_H

#include "tcp_server.h"

class EchoServer {
public:
	EchoServer(unsigned short int type, const char* ip, uint16_t port);

	~EchoServer();

public:
	// 启动服务
	void start();

  /// @brief 处理新客户端连接请求
  void handleNewConnection(Connection* conn);

  /// @brief 关闭客户端的连接
  void handleClose(Connection* conn);

  // 客户端的连接错误
	void handleError(Connection* conn);

  /// @brief 处理客户端的请求报文，在Conncetion类中回调此函数
  void handleMessage(Connection* conn, std::string& msg);

	// 数据发送完成后，在Conncetion类中回调此函数
	void handleSendComplete(Connection* conn);

	// epoll_wait()超时，在EventLoop类中回调此函数
	void handleTimeOut(EventLoop* loop);

private:
	void init();

private:
	TcpServer tcp_serv_;
};

#endif // ECHO_SERVER_H
