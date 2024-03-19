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

class EchoServer {
public:
	EchoServer(unsigned short int type, const char* ip, uint16_t port);

	~EchoServer();

public:
	// 启动服务
	void start();

	// 处理新客户端连接请求，在TcpServer类中回调此函数
	void handleNewConnection(Connection* conn);

	// 关闭客户端的连接，在TcpServer类中回调此函数
	void handleClose(Connection* conn);

	// 客户端的连接错误，在TcpServer中回调此函数
	void handleError(Connection* conn);

	// 处理客户端的请求报文，在TcpServer中回调此函数
	void handleMessage(Connection* conn, std::string& msg);
	
	// 数据发送完成后，在TcpServer中回调此函数
	void handleSendComplete(Connection* conn);

	// epoll_wait()超时，在TcpServer中回调此函数
	void handleTimeOut(EventLoop* loop);

private:
	void init();

private:
	TcpServer tcp_serv_;
};


#endif // ECHO_SERVER_H
