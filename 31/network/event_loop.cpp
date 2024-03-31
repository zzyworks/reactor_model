#include "event_loop.h"

EventLoop::EventLoop() : epo_(std::make_unique<Epoll>()) {}

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
