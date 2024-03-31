#include "channel.h"

Channel::Channel()
    : loop_(nullptr), fd_(-1), events_(0), revents_(0), in_epoll_(false) {}

Channel::Channel(EventLoop* loop, int fd)
    : loop_(loop), fd_(fd), events_(0), revents_(0), in_epoll_(false) {}

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
Channel::disableAll() {
  events_ = 0;
  loop_->undateChannel(this);
}

void
Channel::remove() {
  // 先取消全部的事件
  this->disableAll();
  // 从红黑树上删除fd
  loop_->removeChannel(this);
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
  if (revents_ & EPOLLRDHUP) {  // 对方已关闭，有些系统检测不出
    printf("EPOLLRDHUP\n");
    close_cb_();
  } else if (revents_ & (EPOLLIN | EPOLLPRI)) {  // epoll的就绪队列中有数据可读
    printf("EPOLLIN | EPOLLPRI\n");
    read_cb_();
  } else if (revents_ & EPOLLOUT) {  // 有数据要写
    printf("EPOLLOUT\n");
    write_cb_();
  } else {  // 其他事件
    printf("other\n");
    error_cb_();
  }
}

void
Channel::setReadCallback(ReadCb cb) {
  read_cb_ = std::move(cb);
}

void
Channel::setCloseCallback(CloseCb cb) {
  close_cb_ = std::move(cb);
}

void
Channel::setErrorCallback(ErrorCb cb) {
  error_cb_ = std::move(cb);
}

void
Channel::setWriteCallback(WriteCb cb) {
  write_cb_ = std::move(cb);
}
