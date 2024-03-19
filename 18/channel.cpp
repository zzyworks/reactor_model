#include "channel.h"

int 
Channel::fd() {
  return fd_;
}

void 
Channel::useET() {
  events_ |= EPOLLET;
}

void 
Channel::enableReading() {
  events_ |= EPOLLIN;
  loop_->undateChannel(this);
}

void 
Channel::setInEpoll(bool on) {
 in_epoll_ = on;
}

void 
Channel::setRevents(uint32_t ev) {
  revents_ = ev;
}

bool 
Channel::inPoll() {
  return in_epoll_;
}

uint32_t 
Channel::events() {
  return events_;
}

uint32_t 
Channel::revents() {
  return revents_;
}

void
Channel::handleEvent() {
  if (revents_ & EPOLLRDHUP) { // 对方已关闭，有些系统检测不出
    close_callback_();
  } else if (revents_ & (EPOLLIN | EPOLLPRI)) { // epoll的就绪队列中有数据可读
    read_callback_();
  } else if (revents_ & EPOLLOUT) { // 有数据要写

  } else { // 其他事件
    error_callback_();    
  }
}

void 
Channel::onMessage() {
  char buffer[1024];
  while (true) { // 由于使用非阻塞IO，一次读取buffer大小数据，直到全部的数据读取完毕
    memset(buffer, 0, sizeof(buffer));
    ssize_t read_n = read(fd_, buffer, sizeof(buffer));
    if (read_n > 0) {
      printf("recv(event_fd=%d):%s\n", fd_, buffer);
      send(fd_, buffer, strlen(buffer), 0);
    } else if (read_n == -1 && errno == EINTR) { // 读取数据的时候被信号中断，继续读取
      continue;
    } else if (read_n == -1 
               && ((errno == EAGAIN)
                    || (errno == EWOULDBLOCK))) { // 全部的数据已读取完毕
      break;
    } else if (read_n == 0) { // 客户端已断开
      close_callback_();
      break;
    }
  }
}

void 
Channel::setReadCallback(std::function<void()> fn) {
  read_callback_ = fn;
}

void 
Channel::setCloseCallback(std::function<void()> fn) {
  close_callback_ = fn;
}

void 
Channel::setErrorCallback(std::function<void()> fn) {
  error_callback_ = fn;
}