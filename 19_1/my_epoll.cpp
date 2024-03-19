#include "my_epoll.h"

Epoll::Epoll() : epoll_fd_(-1) {
  this->init_();
}

Epoll::~Epoll() {
  ::close(epoll_fd_);
}

void 
Epoll::updateChannel(Channel* cha) {
  epoll_event ev;
  ev.data.ptr = cha;
  ev.events = cha->events();
  if(cha->inPoll()) { // channel已在红黑树上
    if(epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, cha->fd(), &ev) == -1) {
      printf("epoll_ctl() failed(%d).\n", errno);
      exit(-1);
    }
  } else {
    if(epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, cha->fd(), &ev) == -1) {
      printf("epoll_ctl() failed(%d).\n", errno);
      exit(-1);
    }
    cha->setInEpoll(); // 将channel设置为挂载中
  }
}

void
Epoll::loop(std::vector<Channel*>& channels, int timeout) {
  bzero(events_, sizeof(events_));
  int fds = epoll_wait(epoll_fd_, events_, MAX_EVENTS, timeout);
  if (fds < 0) { // 返回失败
    perror("epoll_wait() failed");
    exit(-1);
  }
  if (fds == 0) { // 超时
    printf("epoll_wait() timeout.\n");
    return;
  }
  Channel* cha = nullptr;
  // 如果fds > 0，表示有事件发生的fd的数量
  for (int i = 0; i != fds; ++i) { // 遍历epoll返回的数组evs
    cha = reinterpret_cast<Channel*>(events_[i].data.ptr);
    cha->setRevents(events_[i].events);
    channels.push_back(cha);
  }
}

void 
Epoll::init_() {
  if((epoll_fd_ = epoll_create(1)) == -1) {
    printf("epoll_create() failed(%d).\n", errno);
    exit(-1);
  }
}