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
Connection::onMessage() {
  char buffer[1024];
  while (true) { // 由于使用非阻塞IO，一次读取buffer大小数据，直到全部的数据读取完毕
    memset(buffer, 0, sizeof(buffer));
    ssize_t read_n = read(this->fd(), buffer, sizeof(buffer));
    if (read_n > 0) {
      input_buffer_.append(buffer, read_n);
    } else if (read_n == -1 && errno == EINTR) { // 读取数据的时候被信号中断，继续读取
      continue;
    } else if (read_n == -1 
               && ((errno == EAGAIN)
                    || (errno == EWOULDBLOCK))) { // 全部的数据已读取完毕
      printf("recv(event_fd=%d):%s\n", this->fd(), input_buffer_.data());
      output_buffer_ = input_buffer_;
      input_buffer_.clear();
      send(this->fd(), output_buffer_.data(), output_buffer_.size(), 0);
      break;
    } else if (read_n == 0) { // 客户端已断开
      this->closeCallback();
      break;
    }
  }
}

void 
Connection::init_() {
  client_channel_ = new Channel(loop_, client_sock_->fd());
  client_channel_->setReadCallback(std::bind(&Connection::onMessage,
                                             this));
  client_channel_->setCloseCallback(std::bind(&Connection::closeCallback,    
                                              this));
  client_channel_->setErrorCallback(std::bind(&Connection::errorCallback,
                                              this));
  client_channel_->useET();
  client_channel_->enableReading();  
}