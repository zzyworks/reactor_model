#include "echo_server.h"

EchoServer::EchoServer(unsigned short int type, const char* ip, uint16_t port,
                       std::size_t sub_thread_n, std::size_t work_thread_n)
    : tcp_serv_(type, ip, port, sub_thread_n),
      sub_thread_n_(sub_thread_n),
      work_thread_n_(work_thread_n),
      thread_pool_(
        std::make_unique<ThreadPool>(work_thread_n_, ThreadType::THREAD_WORK)) {
  this->init();
}

void
EchoServer::start() {
  tcp_serv_.start();
}

void
EchoServer::handleNewConnection(ConnectionPtr conn) {
  printf("New Connection Come in.\n");
}

void
EchoServer::handleClose(ConnectionPtr conn) {
  printf("EchoServer conn closed.\n");
}

void
EchoServer::handleError(ConnectionPtr conn) {
  printf("EchoServer conn error.\n");
}

void
EchoServer::handleMessage(ConnectionPtr conn, std::string& msg) {
  if (work_thread_n_ == 0)
    onMessage(conn, msg);
  else
    thread_pool_->push(std::bind(&EchoServer::onMessage, this, conn, msg));
}

void
EchoServer::handleSendComplete(ConnectionPtr conn) {
  printf("Message send complete.\n");
}

void
EchoServer::handleTimeOut(EventLoop* loop) {
  printf("EchoServer timeout.\n");
}

void
EchoServer::onMessage(ConnectionPtr conn, std::string& msg) {
  msg = "reply:" + msg;
  conn->send(msg.data(), msg.size());  // 把临时缓冲区中的数据发送出去
}

void
EchoServer::init() {
  tcp_serv_.setNewConnectionCb(
    std::bind(&EchoServer::handleNewConnection, this, std::placeholders::_1));
  tcp_serv_.setCloseConnectionCb(
    std::bind(&EchoServer::handleClose, this, std::placeholders::_1));
  tcp_serv_.setErrorConnectionCb(
    std::bind(&EchoServer::handleError, this, std::placeholders::_1));
  tcp_serv_.setOnMessageCb(std::bind(&EchoServer::handleMessage, this,
                                     std::placeholders::_1,
                                     std::placeholders::_2));
  tcp_serv_.setSendCompleteCb(
    std::bind(&EchoServer::handleSendComplete, this, std::placeholders::_1));
  tcp_serv_.setTimeOutCb(
    std::bind(&EchoServer::handleTimeOut, this, std::placeholders::_1));
}
