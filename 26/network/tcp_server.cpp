#include "tcp_server.h"

TcpServer::TcpServer(unsigned short int type, const char* ip, uint16_t port,
                     std::size_t thread_n)
    : main_loop_(nullptr), thread_pool_(nullptr), thread_n_(thread_n) {
  this->init(type, ip, port);
}

TcpServer::~TcpServer() {
  for(auto& ptr : sub_loops_) {
    delete ptr;
    ptr = nullptr;
  }
  for (auto& del : conns_) {
    delete del.second;
    del.second = nullptr;
  }
  if(thread_pool_) {
    delete thread_pool_;
    thread_pool_ = nullptr;
  }
  if (acceptor_) {
    delete acceptor_;
    acceptor_ = nullptr;
  }
  if (main_loop_) {
    delete main_loop_;
    main_loop_ = nullptr;
  }
}

void
TcpServer::newConnection(Socket* client_sock) {
  // 为新客户端连接准备读事件，并添加到epoll中
  Connection* conn =
    new Connection(sub_loops_[client_sock->fd() % thread_n_], client_sock);
  conn->setCloseCallback(
    std::bind(&TcpServer::closeConnection, this, std::placeholders::_1));
  conn->setErrorCallback(
    std::bind(&TcpServer::errorConnection, this, std::placeholders::_1));
  conn->setOnMsgCallback(std::bind(
    &TcpServer::onMessage, this, std::placeholders::_1, std::placeholders::_2));
  conn->setSendCompleteCb(
    std::bind(&TcpServer::sendComplete, this, std::placeholders::_1));
  new_conn_cb_(conn);
  printf("accept client(fd=%d, ip=%s, port=%d) ok.\n", conn->fd(), conn->ip(),
         conn->port());
  conns_[conn->fd()] = conn;
}

void
TcpServer::closeConnection(Connection* conn) {
  close_conn_cb_(conn);
  printf("client(event_fd=%d) disconnected.\n", conn->fd());
  conns_.erase(conn->fd());
  if (conn) {
    delete conn;
    conn = nullptr;
  }
}

void
TcpServer::errorConnection(Connection* conn) {
  error_conn_cb_(conn);
  printf("client(event_fd=%d) error.\n", conn->fd());
  conns_.erase(conn->fd());
  if (conn) {
    delete conn;
    conn = nullptr;
  }
}

void
TcpServer::onMessage(Connection* conn, std::string& msg) {
  on_msg_cb_(conn, msg);
}

void
TcpServer::sendComplete(Connection* conn) {
  send_compl_cb_(conn);
}

void
TcpServer::epollTimeOut(EventLoop* loop) {
  time_out_cb_(loop);
}

void
TcpServer::setNewConnectionCb(std::function<void(Connection*)> fn) {
  new_conn_cb_ = fn;
}

void
TcpServer::setCloseConnectionCb(std::function<void(Connection*)> fn) {
  close_conn_cb_ = fn;
}

void
TcpServer::setErrorConnectionCb(std::function<void(Connection*)> fn) {
  error_conn_cb_ = fn;
}

void
TcpServer::setOnMessageCb(std::function<void(Connection*, std::string&)> fn) {
  on_msg_cb_ = fn;
}

void
TcpServer::setSendCompleteCb(std::function<void(Connection*)> fn) {
  send_compl_cb_ = fn;
}

void
TcpServer::setTimeOutCb(std::function<void(EventLoop*)> fn) {
  time_out_cb_ = fn;
}

void
TcpServer::start() {
  main_loop_->run();
}

void
TcpServer::init(unsigned short int type, const char* ip, uint16_t port) {
  main_loop_ = new EventLoop;
  main_loop_->setEpollTimeOutCb(
    std::bind(&TcpServer::epollTimeOut, this, std::placeholders::_1));
  acceptor_ = new Acceptor(main_loop_, type, ip, port);
  acceptor_->setNewConnectionCb(
    std::bind(&TcpServer::newConnection, this, std::placeholders::_1));
  thread_pool_ = new ThreadPool(thread_n_);       // 创建线程池
  for (std::size_t i = 0; i != thread_n_; ++i) {  // 创建从事件循环
    sub_loops_.push_back(new EventLoop);
    sub_loops_[i]->setEpollTimeOutCb(
      std::bind(&TcpServer::epollTimeOut, this, std::placeholders::_1));
    thread_pool_->push(std::bind(&EventLoop::run, sub_loops_[i]));
  }
}
