/**
 * @file   inet_address.h
 * @author Aklzzz(csdn)
 * @date   2024/03/03 14:54:00
 * @note   
 *  封装socket的地址协议类，用于注册协议族、ip和端口
*/

#ifndef INET_ADDRESS_H
#define INET_ADDRESS_H

#include <arpa/inet.h>
#include <netinet/in.h>

#include "copyable.h"

class InetAddress : Copyable {
public:
  InetAddress() = default;

  /// @brief 用于监听的sock_fd
  explicit InetAddress(unsigned short int type, const char* ip, uint16_t port);

  /// @brief 用于客户端连接的sock_fd
  explicit InetAddress(const sockaddr_in& addr);

  ~InetAddress() = default;

  char* ip();

  const char* ip() const;
  
  uint16_t port() const;

  /// @brief 将sockaddr_in转为sockaddr返回
  const sockaddr* addr() const;

  sockaddr* addr();

private:
  sockaddr_in addr_; // 表示地址协议结构体
};

#endif // INET_ADDRESS_H
