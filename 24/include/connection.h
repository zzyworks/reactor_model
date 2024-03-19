/**
 * @file   connection.h
 * @author Aklzzz(csdn)
 * @date   2024/03/05 11:10:13
 * @note   
 *  封装服务端的client_fd
*/

#ifndef CONNECTION_H
#define CONNECTION_H

#include "channel.h"
#include "event_loop.h"
#include "buffer.h"

class EventLoop;
class Channel;

class Connection : Noncopyable {
public:
  Connection(EventLoop* loop, Socket* client_sock);

  ~Connection();

public:
  /// @brief 返回客户端的fd
  int fd() const;

  /// @brief 返回客户端的ip
  const char* ip() const;

  char* ip();
  
  /// @brief 处理对端发送过来的消息
  void onMessage();
  
  /// 发送数据
  void send(const char* data, size_t n);
  
  /// @brief 返回客户端的port
  uint16_t port() const;

  /// @brief TCP连接关闭（断开）的回调函数，供Channel回调
  void closeCallback();

  /// @brief TCP连接错误的回调函数，供Channel回调
  void errorCallback();

  /// @brief 处理写事件的回调函数，供Channel回调
  void writeCallback();

  /// @brief 设置关闭fd的回调函数
  void setCloseCallback(std::function<void(Connection*)> fn);

  /// @brief 设置fd发生错误的回调函数
  void setErrorCallback(std::function<void(Connection*)> fn);

  /// @brief 设置处理报文的回调函数
  void setOnMsgCallback(std::function<void(Connection*, std::string&)> fn);  

	void setSendCompleteCb(std::function<void(Connection*)> fn);

private:
  void init_();

private:
  EventLoop* loop_;
  Socket* client_sock_; // 服务端用于连接客户端的socket
  Channel* client_channel_; // Connection对应的Channel
  Buffer input_buffer_; // 接收缓冲区
  Buffer output_buffer_; // 发生缓冲区
	// 关闭fd的回调函数
  std::function<void(Connection*)> close_callback_; 
	// fd发生错误的回调函数
  std::function<void(Connection*)> error_callback_; 
	// 处理报文的回调函数
  std::function<void(Connection*, std::string&)> on_msg_callback_; 
	// 发送数据完成后的回调函数，将回调TcpServer::sendComplete()
	std::function<void(Connection*)> send_compl_cb_;
};

#endif // CONNECTION_H
