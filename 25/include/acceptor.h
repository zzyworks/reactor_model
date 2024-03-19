/**
 * @file   acceptor.h
 * @author Aklzzz(csdn)
 * @date   2024/03/04 20:12:45
 * @note   
 *  封装服务端的listen_fd
*/

#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <functional>

#include "channel.h"
#include "event_loop.h"
#include "connection.h"

class Acceptor : Noncopyable {
public:
	typedef std::function<void(Socket*)> NewConnectionCallback;

  Acceptor(EventLoop* loop, unsigned short int type, 
           const char* ip, uint16_t port);
  
	~Acceptor();
  
	/// @brief 处理新客户端连接请求
  void newConnection();
  
	void setNewConnectionCb(const NewConnectionCallback& fn);

private:
  void init(unsigned short int type, const char* ip, uint16_t port);

  EventLoop* loop_;
  Socket* serv_sock_; // 服务端用于监听的socket
  Channel* acceptor_channel_; // Acceptor对应的Channel
  NewConnectionCallback new_conn_cb_; // 处理客户端连接请求的回调函数
};

#endif // ACCEPTOR_H
