/**
 * @file   my_socket.h
 * @author Aklzzz(csdn)
 * @date   2024/03/03 16:33:53
 * @note   
 *  用于封装服务端socket的步骤
 *    1 socket()返回listen_fd
 *    2 bind()
 *    3 listen
 *    4 管理资源的释放
*/

#ifndef MY_SOCKET_H
#define MY_SOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/tcp.h>

#include "inet_address.h"
#include "noncopyable.h"

/// @brief 创建一个非阻塞的socket
int initNonblocking();

class Socket : Noncopyable {
public:
  explicit Socket(int fd);

  ~Socket();

  int fd() const;

  char* ip();

  const char* ip() const;

  uint16_t port() const;

  /// @brief 设置SO_REUSEADDR
  /// @param on true-打开，false-关闭
  void setSOREUSEADDR(bool on);

  /// @brief 设置SO_RESUSEPORT
  /// @param on true-打开，false-关闭
  void setSOREUSEPORT(bool on);

  /// @brief 设置TCP_NODELAY
  /// @param on true-打开，false-关闭
  void setTCPNODELAY(bool on);

  /// @brief 设置SO_KEEPALIVE
  /// @param on true-打开，false-关闭
  void setSOKEEPALIVE(bool on);

  void bind(const InetAddress& addr);

  void listen(int backlog = 128);

  int accept(InetAddress& client_addr);

  void setIpPort(const char* ip, uint16_t port);

private:
  int fd_;
  char* ip_; // 如果是listen_fd，存放服务端监听的ip；如果是client_fd，存放对端的ip
  uint16_t port_; // 如果是listen_fd，存放服务端监听的port；如果是client_fd，存放外部端口
};

#endif // MY_SOCKET_H
