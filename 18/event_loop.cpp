#include "event_loop.h"

EventLoop::EventLoop() : epo_(new Epoll) {}

EventLoop::~EventLoop() {
  if(epo_) {
    delete epo_;
    epo_ = nullptr;
  }
}

void
EventLoop::run(std::vector<Channel*>& channels) {
  while (true) {
    epo_->loop(channels);
    for (Channel*& cha : channels) { // 遍历epoll返回的数组evs
      cha->handleEvent();
    }
    channels.clear();
  }
}

Epoll* 
EventLoop::epo() {
  return epo_;
}

void 
EventLoop::undateChannel(Channel* cha) {
  epo_->updateChannel(cha);
}