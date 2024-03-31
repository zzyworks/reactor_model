#include "event_loop.h"

EventLoop::EventLoop()
    : epo_(std::make_unique<Epoll>()),
      thread_id_(0),
      wake_up_fd_(eventfd(0, EFD_NONBLOCK)),
      wake_channel_(std::make_unique<Channel>(this, wake_up_fd_)) {
  wake_channel_->setReadCallback(std::bind(&EventLoop::handleWakeUp, this));
  wake_channel_->enableReading();
}

void
EventLoop::run() {
  thread_id_ = syscall(SYS_gettid);
  while (true) {
    std::vector<Channel*> channels = epo_->loop();
    if (channels.size() == 0)
      epo_timeout_cb_(this);
    else {
      // 遍历epoll返回的数组evs
      for (Channel*& cha : channels)
        cha->handleEvent();
    }
  }
}

void
EventLoop::push(Tasks fn) {
  {
    std::lock_guard<std::mutex> lock(mutex_);
    tasks_que_.push(fn);
  }
  // 唤醒时间循环
  this->wakeUp();
}

void
EventLoop::undateChannel(Channel* cha) {
  epo_->updateChannel(cha);
}

void
EventLoop::removeChannel(Channel* cha) {
  epo_->removeChannel(cha);
}

void
EventLoop::setEpollTimeOutCb(EpollTimeoutCb cb) {
  epo_timeout_cb_ = std::move(cb);
}

bool
EventLoop::isInLoopThread() {
  return thread_id_ == syscall(SYS_gettid);
}

void
EventLoop::wakeUp() {
  uint64_t val = 1;
  write(wake_up_fd_, &val, sizeof(val));
}

void
EventLoop::handleWakeUp() {
  printf("EventLoop::handleWakeUp() thread is %ld.\n", syscall(SYS_gettid));
  uint64_t val = 0;
  read(wake_up_fd_, &val, sizeof(val));
  Tasks fn;
  std::lock_guard<std::mutex> lock(mutex_);
  while (tasks_que_.size() > 0) {
    fn = std::move(tasks_que_.front());
    tasks_que_.pop();
    fn();
  }
}
