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
      while (true) {
        //////////////////////////////////////////////////////
        // 可以把以下代码封装在Buffer类中，还可以支持固定长度，指定报文长度和分隔符等多种格式
        size_t len;
        memcpy(&len, input_buffer_.data(), 4); // 从input_buffer_中获取报文头部
        // 如果input_buffer_中的数据量小于报文头部，说明input_buffer_中的报文不完整
        if(input_buffer_.size() < len + 4) break;

        std::string msg(input_buffer_.data() + 4, len); // 从input_buffer_中获取一个报文
        input_buffer_.erase(0, len + 4); // 从input_buffer_中删除刚才已经获取的报文
        //////////////////////////////////////////////////////

        printf("msg (event_fd=%d):%s\n", this->fd(), msg.data());
        on_msg_callback_(this, msg);
      }
      break;
    } else if (read_n == 0) { // 客户端已断开
      this->closeCallback();
      break;
    }
  }
}


void 
Connection::send(const char* data, size_t n) {
  output_buffer_.append(data, n); // 把需要发送的数据保存到Connection的发送缓冲区中
  // 注册写事件
  client_channel_->enableWriting();
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
Connection::writeCallback() {
  // 尝试把output_buffer_中的数据全部发送出去
  int writen = ::send(this->fd(), output_buffer_.data(),
                      output_buffer_.size(), 0); 
	// 从output_buffer_中删除已成功发送的字节数
	if(writen > 0) output_buffer_.erase(0, writen); 
	// 如果发生缓冲区中没有数据了，表示数据已发送成功，不在关注写事件
	if(output_buffer_.size() == 0) {
		client_channel_->disableWrite();
		send_comp_callback_(this);
	} 
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
Connection::setOnMsgCallback(std::function<void(Connection*, 
                                                std::string&)> fn) {
  on_msg_callback_ = fn;
}


void
Connection::setSendCompCallback(std::function<void(Connection*)> fn) {
	send_comp_callback_ = fn;
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
	client_channel_->setWriteCallback(std::bind(&Connection::writeCallback,
																							this));
  client_channel_->useET(); // 采用边缘触发
  client_channel_->enableReading();  
}
