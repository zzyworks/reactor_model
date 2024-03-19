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
  conn->setOnMsgCallback(std::bind(&TcpServer::onMessage,
                                   this,
                                   std::placeholders::_1,
                                   std::placeholders::_2));
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
TcpServer::onMessage(Connection* conn, std::string& msg) {
  // 从input_buffer_拷贝到output_buffer_中...
  msg = "reply:" + msg;

  int len = msg.size(); // 计算回应报文的大小
  std::string tmp_buf((char*)&len, 4); // 把报文头部填充到回应报文中
  tmp_buf.append(msg); // 把报文内容填充到回应报文中

  // send(conn->fd(), tmp_buf.data(), tmp_buf.size(), 0); // 把临时缓冲区中的数据直接send()出去
  conn->send(tmp_buf.data(), tmp_buf.size()); // 把临时缓冲区中的数据发送出去
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