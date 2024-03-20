#include "my_socket.h"

Socket::Socket(int fd)
: fd_(fd), ip_(nullptr), port_(0) {}

int initNonblocking() {
  int listen_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
  if (listen_fd < 0) {
    perror("socket() failed.\n");
    exit(-1);
  }
  return listen_fd;
}

Socket::~Socket() {
 ::close(fd_);
}

int 
Socket::fd() const {
  return fd_;
}

char* 
Socket::ip() {
  return ip_;
}

const char* 
Socket::ip() const {
  return ip_;
}

uint16_t 
Socket::port() const {
  return port_;
}

void 
Socket::setSOREUSEADDR(bool on) {
  unsigned int opt_val = on ? 1 : 0;
  ::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val));
}

void 
Socket::setSOREUSEPORT(bool on) {
  unsigned int opt_val = on ? 1 : 0;
  ::setsockopt(fd_, SOL_SOCKET, SO_REUSEPORT, &opt_val, sizeof(opt_val));
}

void 
Socket::setTCPNODELAY(bool on) {
  unsigned int opt_val = on ? 1 : 0;
  ::setsockopt(fd_, SOL_SOCKET, TCP_NODELAY, &opt_val, sizeof(opt_val));
}

void 
Socket::setSOKEEPALIVE(bool on) {
  unsigned int opt_val = on ? 1 : 0;
  ::setsockopt(fd_, SOL_SOCKET, SO_KEEPALIVE, &opt_val, sizeof(opt_val));
}

void 
Socket::bind(const InetAddress& addr) {
  if (::bind(fd_, addr.addr(), sizeof(addr)) < 0) {
    perror("bind() failed");
    ::close(fd_);
    exit(-1);
  }
  this->setIpPort(addr.ip(), addr.port());
}

void 
Socket::listen(int backlog) {
  if (::listen(fd_, backlog) != 0) {
    perror("listen() failed");
    close(fd_);
    exit(-1);
  }
}

int
Socket::accept(InetAddress& client_addr) {
  socklen_t len = sizeof(client_addr);
  int client_fd = accept4(fd_, client_addr.addr(), &len, SOCK_NONBLOCK);
  return client_fd;
}

void 
Socket::setIpPort(const char* ip, uint16_t port) {
  ip_ = const_cast<char*>(ip);
  port_ = port;
}
