#include "acceptor.h"

Acceptor::Acceptor(EventLoop* loop, unsigned short int type, const char* ip,
                   uint16_t port)
    : loop_(loop), serv_sock_(std::make_unique<Socket>(initNonblocking())) {
  this->init(type, ip, port);
}

void
Acceptor::newConnection() {
  InetAddress client_addr;
  // client_sock只能new出来，不能在栈上，否则析构函数会关闭fd
  auto client_sock = std::make_unique<Socket>(serv_sock_->accept(client_addr)); 
  client_sock->setIpPort(client_addr.ip(), client_addr.port());
  new_conn_cb_(std::move(client_sock));  // 回调TcpServer::newConnection()
}

void
Acceptor::setNewConnectionCb(NewConnectionCb fn) {
  new_conn_cb_ = std::move(fn);
}

void
Acceptor::init(unsigned short int type, const char* ip, uint16_t port) {
  InetAddress addr(type, ip, port);
  serv_sock_->setSOREUSEADDR(true);
  serv_sock_->setSOREUSEPORT(true);
  serv_sock_->setTCPNODELAY(true);
  serv_sock_->setSOKEEPALIVE(true);
  serv_sock_->bind(addr);
  serv_sock_->listen();

  acceptor_channel_ = std::make_unique<Channel>(loop_, serv_sock_->fd());
  acceptor_channel_->setReadCallback(std::bind(&Acceptor::newConnection, this));
  acceptor_channel_->enableReading();
}
