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
	conn->setSendCompCallback(std::bind(&TcpServer::sendComplete,
																			this,
																			std::placeholders::_1));
	printf ("new connection(fd=%d,ip=%s,port=%d) ok.\n",conn->fd(),conn->ip(),conn->port());
  conns_[conn->fd()] = conn;
	new_conn_cb_(conn);
}

void 
TcpServer::closeConnection(Connection* conn) {
	if(close_conn_cb_) close_conn_cb_(conn);
  conns_.erase(conn->fd());
  if(conn) {
    delete conn;
    conn = nullptr;
  }
}

void 
TcpServer::errorConnection(Connection* conn) {
	if(error_conn_cb_) error_conn_cb_(conn);
  conns_.erase(conn->fd());
  if(conn) {
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
TcpServer::epolltimeout(EventLoop* loop) {
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
	loop_.setEpollTOCallbakc(std::bind(&TcpServer::epolltimeout,
																		 this,
																		 std::placeholders::_1));
}
