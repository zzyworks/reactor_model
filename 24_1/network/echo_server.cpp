#include "echo_server.h"

EchoServer::EchoServer(unsigned short int type, const char* ip, uint16_t port)
    : tcp_serv_(type, ip, port) {
  this->init();
}

EchoServer::~EchoServer() {}

void
EchoServer::start() {
  tcp_serv_.start();
}

void
EchoServer::handleNewConnection(Connection* conn) {
  printf("New Connection Come in.\n");
}

void
EchoServer::handleClose(Connection* conn) {
  printf("EchoServer conn closed.\n");
}

void
EchoServer::handleError(Connection* conn) {
  printf("EchoServer conn error.\n");
}

void
EchoServer::handleMessage(Connection* conn, std::string& msg) {
  // 从input_buffer_拷贝到output_buffer_中...
  msg = "reply:" + msg;

  conn->send(msg.data(), msg.size());  // 把临时缓冲区中的数据发送出去
}

void
EchoServer::handleSendComplete(Connection* conn) {
  printf("Message send complete.\n");
}

void
EchoServer::handleTimeOut(EventLoop* loop) {
  printf("EchoServer timeout.\n");
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
