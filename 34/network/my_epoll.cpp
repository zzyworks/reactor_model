#include "my_epoll.h"

Epoll::Epoll() : epoll_fd_(-1) {
  this->init();
}

Epoll::~Epoll() {
  ::close(epoll_fd_);
}

void
Epoll::updateChannel(Channel* cha) {
  epoll_event ev;
  ev.data.ptr = cha;
  ev.events = cha->events();
  if (cha->inPoll()) {  // channel已在红黑树上
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, cha->fd(), &ev) == -1) {
      printf("epoll_ctl() failed(%d).\n", errno);
      exit(-1);
    }
  } else {
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, cha->fd(), &ev) == -1) {
      printf("epoll_ctl() failed(%d).\n", errno);
      exit(-1);
    }
    cha->setInEpoll();  // 将channel设置为挂载中
  }
}

void
Epoll::removeChannel(Channel* cha) {
  if (cha->inPoll()) {  // channel已在红黑树上
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, cha->fd(), 0) == -1) {
      printf("epoll_ctl() failed(%d).\n", errno);
      exit(-1);
    }
  }
}

std::vector<Channel*>
Epoll::loop(int timeout) {
  std::vector<Channel*> channels;
  bzero(events_, sizeof(events_));
  int fds = epoll_wait(epoll_fd_, events_, MAX_EVENTS, timeout);
  if (fds < 0) {  // 返回失败
                  // EBADF: ep_fd不是一个有效的描述符
    // EFAULT:参数events指向的内存区域不可写
    // EINVAL：ep_fd不是一个epoll文件描述符，或者参数maxevents小于等于0
    // EINTR：阻塞过程中被信号中断，eoll_pwait()可以避免，或者错误处理中，解析error后重新调用epoll_wait()
    perror("epoll_wait() failed");
    exit(-1);
  }
  if (fds == 0) {  // 超时
    return channels;
  }
  Channel* cha = nullptr;
  // 如果fds > 0，表示有事件发生的fd的数量
  for (int i = 0; i != fds; ++i) {  // 遍历epoll返回的数组evs
    cha = reinterpret_cast<Channel*>(events_[i].data.ptr);
    cha->setRevents(events_[i].events);
    channels.push_back(cha);
  }
  return channels;
}

void
Epoll::init() {
  if ((epoll_fd_ = epoll_create(1)) == -1) {
    printf("epoll_create() failed(%d).\n", errno);
    exit(-1);
  }
}
