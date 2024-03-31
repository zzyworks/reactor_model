#include "tcp_server.h"

TcpServer::TcpServer(unsigned short int type, const char* ip, uint16_t port,
                     std::size_t thread_n)
    : main_loop_(std::make_unique<EventLoop>(LoopType::MAIN_LOOP)),
      thread_n_(thread_n),
      thread_pool_(
        std::make_unique<ThreadPool>(thread_n_, ThreadType::THREAD_IO)) {
  this->init(type, ip, port);
}

void
TcpServer::start() {
  main_loop_->run();
}

void 
TcpServer::stop() {
  // 停止主事件循环
  main_loop_->stop();
  printf("主事件循环已停止\n");
  // 停止从事件循环
  for(std::unique_ptr<EventLoop>& ptr : sub_loops_)
    ptr->stop();
  printf("从事件循环已停止\n");
  // 停止IO线程
  thread_pool_->destroy();
  printf("IO线程已停止\n");
}

void
TcpServer::removeConnection(int fd) {
  std::lock_guard<std::mutex> lock(mutex_conns_);
  conns_.erase(fd);
}

void
TcpServer::newConnection(std::unique_ptr<Socket> client_sock) {
  // 为新客户端连接准备读事件，并添加到epoll中
  ConnectionPtr conn = std::make_shared<Connection>(
    sub_loops_[client_sock->fd() % thread_n_].get(), std::move(client_sock));
  conn->setCloseCallback(
    std::bind(&TcpServer::closeConnection, this, std::placeholders::_1));
  conn->setErrorCallback(
    std::bind(&TcpServer::errorConnection, this, std::placeholders::_1));
  conn->setOnMsgCallback(std::bind(
    &TcpServer::onMessage, this, std::placeholders::_1, std::placeholders::_2));
  conn->setSendCompleteCb(
    std::bind(&TcpServer::sendComplete, this, std::placeholders::_1));
  new_conn_cb_(conn);
  {
    std::lock_guard<std::mutex> lock(mutex_conns_);
    conns_[conn->fd()] = conn;
  }
  sub_loops_[conn->fd() % thread_n_]->pushNewConnection(conn);
}

void
TcpServer::closeConnection(ConnectionPtr conn) {
  close_conn_cb_(conn);
  {
    std::lock_guard<std::mutex> lock(mutex_conns_);
    conns_.erase(conn->fd());
  }
}

void
TcpServer::errorConnection(ConnectionPtr conn) {
  error_conn_cb_(conn);
  {
    std::lock_guard<std::mutex> lock(mutex_conns_);
    conns_.erase(conn->fd());
  }
}

void
TcpServer::onMessage(ConnectionPtr conn, std::string& msg) {
  on_msg_cb_(conn, msg);
}

void
TcpServer::sendComplete(ConnectionPtr conn) {
  send_compl_cb_(conn);
}

void
TcpServer::epollTimeOut(EventLoop* loop) {
  time_out_cb_(loop);
}

void
TcpServer::setNewConnectionCb(NewConnCb fn) {
  new_conn_cb_ = std::move(fn);
}

void
TcpServer::setCloseConnectionCb(CloseConnCb fn) {
  close_conn_cb_ = std::move(fn);
}

void
TcpServer::setErrorConnectionCb(ErrorConnCb fn) {
  error_conn_cb_ = std::move(fn);
}

void
TcpServer::setOnMessageCb(OnMsgCb fn) {
  on_msg_cb_ = std::move(fn);
}

void
TcpServer::setSendCompleteCb(SendComplCb fn) {
  send_compl_cb_ = std::move(fn);
}

void
TcpServer::setTimeOutCb(std::function<void(EventLoop*)> fn) {
  time_out_cb_ = std::move(fn);
}

void
TcpServer::init(unsigned short int type, const char* ip, uint16_t port) {
  // 创建主事件循环
  main_loop_->setEpollTimeOutCb(
    std::bind(&TcpServer::epollTimeOut, this, std::placeholders::_1));
  acceptor_ = std::make_unique<Acceptor>(main_loop_.get(), type, ip, port);
  acceptor_->setNewConnectionCb(
    std::bind(&TcpServer::newConnection, this, std::placeholders::_1));
  // 创建从事件循环
  for (std::size_t i = 0; i != thread_n_; ++i) {  // 创建从事件循环
    sub_loops_.push_back(
      std::make_unique<EventLoop>(LoopType::OTHER_LOOP, 5, 10));
    sub_loops_[i]->setEpollTimeOutCb(
      std::bind(&TcpServer::epollTimeOut, this, std::placeholders::_1));
    sub_loops_[i]->setTimerCb(
      std::bind(&TcpServer::removeConnection, this, std::placeholders::_1));
    thread_pool_->push(std::bind(&EventLoop::run, sub_loops_[i].get()));
  }
}
