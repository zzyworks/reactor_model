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
Channel::disableReading() {
  events_ &= ~EPOLLIN;
  loop_->undateChannel(this);
}

void 
Channel::enableWriting() {
  events_ |= EPOLLOUT;
  loop_->undateChannel(this);
}

void 
Channel::disableWrite() {
  events_ &= ~EPOLLOUT;
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
		write_callback_();
  } else { // 其他事件
    error_callback_();    
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

void
Channel::setWriteCallback(std::function<void()> fn) {
	write_callback_ = fn;
}
