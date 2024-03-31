#include "acceptor.h"

Acceptor::Acceptor(EventLoop* loop, unsigned short int type, const char* ip,
                   uint16_t port)
    : loop_(loop) {
  this->init(type, ip, port);
}

Acceptor::~Acceptor() {
  if (serv_sock_) {
    delete serv_sock_;
    serv_sock_ = nullptr;
  }
  if (acceptor_channel_) {
    delete acceptor_channel_;
    acceptor_channel_ = nullptr;
  }
}

void
Acceptor::newConnection() {
  InetAddress client_addr;
  // client_sock只能new出来，不能在栈上，否则析构函数会关闭fd
  Socket* client_sock = new Socket(serv_sock_->accept(client_addr));
  client_sock->setIpPort(client_addr.ip(), client_addr.port());
  new_conn_cb_(client_sock);  // 回调TcpServer::newConnection()
}

void
Acceptor::setNewConnectionCb(const NewConnectionCallback& fn) {
  new_conn_cb_ = fn;
}

void
Acceptor::init(unsigned short int type, const char* ip, uint16_t port) {
  serv_sock_ = new Socket(initNonblocking());
  InetAddress addr(type, ip, port);
  serv_sock_->setSOREUSEADDR(true);
  serv_sock_->setSOREUSEPORT(true);
  serv_sock_->setTCPNODELAY(true);
  serv_sock_->setSOKEEPALIVE(true);
  serv_sock_->bind(addr);
  serv_sock_->listen();

  acceptor_channel_ = new Channel(loop_, serv_sock_->fd());
  acceptor_channel_->setReadCallback(std::bind(&Acceptor::newConnection, this));
  acceptor_channel_->enableReading();
}
