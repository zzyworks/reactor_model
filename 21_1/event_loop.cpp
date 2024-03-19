#include "event_loop.h"

EventLoop::EventLoop() : epo_(new Epoll) {}

EventLoop::~EventLoop() {
  if(epo_) {
    delete epo_;
    epo_ = nullptr;
  }
}

void
EventLoop::run() {
  while (true) {
		std::vector<Channel*> channels = epo_->loop();
		// 遍历epoll返回的数组evs
    for (Channel*& cha : channels) cha->handleEvent();
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
