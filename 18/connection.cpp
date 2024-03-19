#include "connection.h"

Connection::Connection(EventLoop* loop, 
                       Socket* client_sock) : loop_(loop),
                                              client_sock_(client_sock){
  this->init_();
}

Connection::~Connection() {
  if(client_channel_) {
    delete client_channel_;
    client_channel_ = nullptr;
  }
  if(client_sock_) {
    delete client_sock_;
    client_sock_ = nullptr;
  }
}

int 
Connection::fd() const {
  return client_sock_->fd();
}

const char* 
Connection::ip() const {
  return client_sock_->ip();
}

char* 
Connection::ip() {
  return client_sock_->ip();
}

uint16_t
Connection::port() const {
  return client_sock_->port();
}

void 
Connection::closeCallback() {
  close_callback_(this);
}

void 
Connection::errorCallback() {
  error_callback_(this);  
}

void 
Connection::setCloseCallback(std::function<void(Connection*)> fn) {
  close_callback_ = fn;
}

void 
Connection::setErrorCallback(std::function<void(Connection*)> fn) {
  error_callback_ = fn;
}

void 
Connection::init_() {
  client_channel_ = new Channel(loop_, client_sock_->fd());
  client_channel_->setReadCallback(std::bind(&Channel::onMessage,
                                             client_channel_));
  client_channel_->setCloseCallback(std::bind(&Connection::closeCallback,    
                                              this));
  client_channel_->setErrorCallback(std::bind(&Connection::errorCallback,
                                              this));
  client_channel_->useET();
  client_channel_->enableReading();  
}