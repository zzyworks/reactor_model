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

  /// @brief 返回客户端的port
  uint16_t port() const;

  /// @brief TCP连接关闭（断开）的回调函数，供Channel回调
  void closeCallback();

  /// @brief TCP连接错误的回调函数，供Channel回调
  void errorCallback();

  /// @brief 设置关闭fd的回调函数
  void setCloseCallback(std::function<void(Connection*)> fn);

  /// @brief 设置fd发生错误的回调函数
  void setErrorCallback(std::function<void(Connection*)> fn);

  /// @brief 处理对端发送过来的消息
  void onMessage();

private:
  void init_();

private:
  EventLoop* loop_;
  Socket* client_sock_; // 服务端用于连接客户端的socket
  Channel* client_channel_; // Connection对应的Channel
  Buffer input_buffer_; // 接收缓冲区
  Buffer output_buffer_; // 发生缓冲区
  std::function<void(Connection*)> close_callback_; // 关闭fd的回调函数
  std::function<void(Connection*)> error_callback_; // fd发生错误的回调函数
};

#endif // CONNECTION_H