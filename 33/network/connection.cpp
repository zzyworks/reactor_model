#include "connection.h"

Connection::Connection(EventLoop* loop, std::unique_ptr<Socket> client_sock)
    : loop_(loop),
      client_sock_(std::move(client_sock)),
      client_channel_(std::make_unique<Channel>(loop_, client_sock_->fd())),
      disconn_(false) {
  this->init();
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
  while (
    true) {  // 由于使用非阻塞IO，一次读取buffer大小数据，直到全部的数据读取完毕
    memset(buffer, 0, sizeof(buffer));
    ssize_t read_n = read(this->fd(), buffer, sizeof(buffer));
    if (read_n > 0) {
      input_buffer_.append(buffer, read_n);
    } else if (read_n == -1
               && errno == EINTR) {  // 读取数据的时候被信号中断，继续读取
      continue;
    } else if (read_n == -1
               && ((errno == EAGAIN)
                   || (errno == EWOULDBLOCK))) {  // 全部的数据已读取完毕
      while (true) {
        //////////////////////////////////////////////////////
        // 可以把以下代码封装在Buffer类中，还可以支持固定长度，指定报文长度和分隔符等多种格式
        int len;
        memcpy(&len, input_buffer_.data(), 4);  // 从input_buffer_中获取报文头部
        // 如果input_buffer_中的数据量小于报文头部，说明input_buffer_中的报文不完整
        if (input_buffer_.size() < len + 4)
          break;

        std::string msg(input_buffer_.data() + 4,
                        len);  // 从input_buffer_中获取一个报文
        input_buffer_.erase(
          0, len + 4);  // 从input_buffer_中删除刚才已经获取的报文
        //////////////////////////////////////////////////////
        printf("msg (event_fd=%d):%s\n", this->fd(), msg.data());
        last_time_ = TimeStamp::now();
        on_msg_cb_(shared_from_this(), msg);
      }
      break;
    } else if (read_n == 0) {  // 客户端已断开
      this->closeCallback();
      break;
    }
  }
}

void
Connection::send(const char* data, size_t n) {
  printf("Connection::send() thread is %ld.\n", syscall(SYS_gettid));
  if (disconn_) {
    printf("client connection was closed.\n");
    return;
  }
  if (loop_->isInLoopThread()) {
    // 如果当前线程是IO线程，直接调用sendToBuffer发送数据
    printf("事件循环的线程\n");
    this->sendToBuffer(data, n);
  } else {
    // 如果当前线程不是IO线程，调用EventLoop::push()，把sendToBuffer交给事件循环线程去执行
    printf("不是事件循环的线程\n");
    loop_->push(std::bind(&Connection::sendToBuffer, this, data, n));
  }
}

void
Connection::closeCallback() {
  disconn_ = true;
  client_channel_->remove();
  close_cb_(shared_from_this());
}

void
Connection::errorCallback() {
  disconn_ = true;
  client_channel_->remove();
  error_cb_(shared_from_this());
}

void
Connection::writeCallback() {
  printf("Connection::writeCallback() thread is %ld.\n", syscall(SYS_gettid));
  // 尝试把output_buffer_中的数据全部发送出去
  int writen =
    ::send(this->fd(), output_buffer_.data(), output_buffer_.size(), 0);
  // 从output_buffer_中删除已成功发送的字节数
  if (writen > 0)
    output_buffer_.erase(0, writen);
  // 如果发生缓冲区中没有数据了，表示数据已发送成功，不在关注写事件
  if (output_buffer_.size() == 0) {
    client_channel_->disableWrite();
    send_compl_cb_(shared_from_this());
  }
}

void
Connection::setCloseCallback(CloseCb fn) {
  close_cb_ = std::move(fn);
}

void
Connection::setErrorCallback(ErrorCb fn) {
  error_cb_ = std::move(fn);
}

void
Connection::setOnMsgCallback(OnMsgCb fn) {
  on_msg_cb_ = std::move(fn);
}

void
Connection::setSendCompleteCb(SendCompleteCb fn) {
  send_compl_cb_ = std::move(fn);
}

void
Connection::init() {
  client_channel_->setReadCallback(std::bind(&Connection::onMessage, this));
  client_channel_->setCloseCallback(
    std::bind(&Connection::closeCallback, this));
  client_channel_->setErrorCallback(
    std::bind(&Connection::errorCallback, this));
  client_channel_->setWriteCallback(
    std::bind(&Connection::writeCallback, this));
  client_channel_->useET();  // 采用边缘触发
  client_channel_->enableReading();
}

void
Connection::sendToBuffer(const char* data, size_t n) {
  // 把需要发送的数据保存到Connection的发送缓冲区中
  output_buffer_.appendWithHead(data, n);
  // 注册写事件
  client_channel_->enableWriting();
}
