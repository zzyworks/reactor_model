#include "inet_address.h"

InetAddress::InetAddress(unsigned short int type,
                         const char* ip, 
                         uint16_t port) {
  addr_.sin_family = type;
  addr_.sin_addr.s_addr = inet_addr(ip);
  addr_.sin_port = htons(port);
}

InetAddress::InetAddress(const sockaddr_in& addr) : addr_(addr) {}

char* 
InetAddress::ip() {
  return inet_ntoa(addr_.sin_addr);  
}

const char* 
InetAddress::ip() const {
 return inet_ntoa(addr_.sin_addr);
}

uint16_t 
InetAddress::port() const {
  return ntohs(addr_.sin_port);
}

const sockaddr* 
InetAddress::addr() const {
  return reinterpret_cast<const sockaddr*>(&addr_);
}

sockaddr* 
InetAddress::addr() {
  return reinterpret_cast<sockaddr*>(&addr_);
}