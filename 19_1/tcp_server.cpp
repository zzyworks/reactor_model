#include "tcp_server.h"

TcpServer::TcpServer(unsigned short int type, const char* ip, uint16_t port) {
  this->init_(type, ip, port);
}

TcpServer::~TcpServer() {
  if(acceptor_) {
    delete acceptor_;
    acceptor_ = nullptr;
  }
  for(auto& del : conns_) {
    delete del.second;
    del.second = nullptr;
  }
}

void
TcpServer::newConnection(Socket* client_sock) {
  // 为新客户端连接准备读事件，并添加到epoll中
  Connection* conn = new Connection(&loop_, client_sock);
  conn->setCloseCallback(std::bind(&TcpServer::closeConnection,
                                   this,
                                   std::placeholders::_1));
  conn->setErrorCallback(std::bind(&TcpServer::errorConnection,
                                   this,
                                   std::placeholders::_1));
  printf("accept client(fd=%d, ip=%s, port=%d) ok.\n",
         conn->fd(), conn->ip(), conn->port());
  conns_[conn->fd()] = conn;
}

void 
TcpServer::closeConnection(Connection* conn) {
  printf("client(event_fd=%d) disconnected.\n", conn->fd());
  conns_.erase(conn->fd());
  if(conn) {
    delete conn;
    conn = nullptr;
  }
}

void 
TcpServer::errorConnection(Connection* conn) {
  printf("client(event_fd=%d) error.\n", conn->fd());
  conns_.erase(conn->fd());
  if(conn) {
    delete conn;
    conn = nullptr;
  }
}

void 
TcpServer::start() {
  std::vector<Channel*> channels;
  channels.reserve(8);
  loop_.run(channels);
}

void
TcpServer::init_(unsigned short int type, const char* ip, uint16_t port) {
  acceptor_ = new Acceptor(&loop_, type, ip, port);
  acceptor_->setNewConnectionCb(std::bind(&TcpServer::newConnection,
                                          this, 
                                          std::placeholders::_1));
}