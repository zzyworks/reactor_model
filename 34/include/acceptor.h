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
#include <memory>

#include "channel.h"
#include "my_socket.h"
#include "connection.h"
#include "event_loop.h"

class Acceptor : Noncopyable {
public:
  using NewConnectionCb = std::function<void(std::unique_ptr<Socket>)>;

  Acceptor(EventLoop* loop, unsigned short int type, const char* ip,
           uint16_t port);

  ~Acceptor() = default;

  /// @brief 处理新客户端连接请求
  void newConnection();

  void setNewConnectionCb(NewConnectionCb fn);

private:
  void init(unsigned short int type, const char* ip, uint16_t port);

  EventLoop* loop_;  // Acceptor对应的事件循环，在构造函数中传入
  std::unique_ptr<Socket>
    serv_sock_;  // 服务端用于监听的socket，在构造函数中创建
  std::unique_ptr<Channel>
    acceptor_channel_;  // Acceptor对应的Channel，在构造函数中创建
  NewConnectionCb new_conn_cb_;  // 处理客户端连接请求的回调函数
};

#endif  // ACCEPTOR_H
