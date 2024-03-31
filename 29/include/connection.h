/**
 * @file   connection.h
 * @author Aklzzz(csdn)
 * @date   2024/03/05 11:10:13
 * @note   
 *  封装服务端的client_fd
*/

#ifndef CONNECTION_H
#define CONNECTION_H

#include <atomic>
#include <memory>

#include "buffer.h"
#include "event_loop.h"
#include "noncopyable.h"

class EventLoop;
class Socket;
class Channel;

class Connection : public std::enable_shared_from_this<Connection>,
                   public Noncopyable {
public:
  using ConnectionPtr = std::shared_ptr<Connection>;
  using CloseCb = std::function<void(ConnectionPtr)>;
  using ErrorCb = std::function<void(ConnectionPtr)>;
  using OnMsgCb = std::function<void(ConnectionPtr, std::string&)>;
  using SendCompleteCb = std::function<void(ConnectionPtr)>;

  Connection(EventLoop* loop, Socket* client_sock);

  ~Connection();

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
  void setCloseCallback(CloseCb fn);

  /// @brief 设置fd发生错误的回调函数
  void setErrorCallback(ErrorCb fn);

  /// @brief 设置处理报文的回调函数
  void setOnMsgCallback(OnMsgCb fn);

  void setSendCompleteCb(SendCompleteCb fn);

private:
  void init();

  EventLoop* loop_;
  Socket* client_sock_;      // 服务端用于连接客户端的socket
  Channel* client_channel_;  // Connection对应的Channel
  Buffer input_buffer_;      // 接收缓冲区
  Buffer output_buffer_;     // 发生缓冲区
  CloseCb close_cb_;         // 关闭fd的回调函数
  ErrorCb error_cb_;         // fd发生错误的回调函数
  OnMsgCb on_msg_cb_;        // 处理报文的回调函数
  SendCompleteCb
    send_compl_cb_;  // 发送数据完成后的回调函数，将回调TcpServer::sendComplete()
  std::atomic_bool disconn_;  // 客户端连接是否已断开，true-断开
};

#endif  // CONNECTION_H
