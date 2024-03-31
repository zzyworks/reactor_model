#include "event_loop.h"

int
initTimerFd(int sec) {
  int tfd = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC | TFD_NONBLOCK);
  itimerspec timeout;
  bzero(&timeout, sizeof(timeout));
  timeout.it_value.tv_sec = sec;
  timeout.it_value.tv_nsec = 0;
  timerfd_settime(tfd, 0, &timeout, 0);
  return tfd;
}

EventLoop::EventLoop(LoopType loop_type, time_t time_val, time_t timeout)
    : loop_type_(loop_type),
      epo_(std::make_unique<Epoll>()),
      thread_id_(0),
      wake_up_fd_(eventfd(0, EFD_NONBLOCK)),
      wake_channel_(std::make_unique<Channel>(this, wake_up_fd_)),
      time_val_(time_val),
      timeout_(timeout),
      timer_fd_(initTimerFd(time_val_)),
      timer_channel_(std::make_unique<Channel>(this, timer_fd_)),
      stop_(false) {
  wake_channel_->setReadCallback(std::bind(&EventLoop::handleWakeUp, this));
  wake_channel_->enableReading();
  timer_channel_->setReadCallback(std::bind(&EventLoop::handleTimer, this));
  timer_channel_->enableReading();
}

void
EventLoop::run() {
  thread_id_ = syscall(SYS_gettid);
  while (!stop_) {
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
EventLoop::stop() {
  stop_ = true;
  this->wakeUp();
}

void
EventLoop::pushTask(Tasks fn) {
  {
    std::lock_guard<std::mutex> lock(mutex_);
    tasks_que_.push(fn);
  }
  // 唤醒时间循环
  this->wakeUp();
}

void
EventLoop::pushNewConnection(ConnectionPtr conn) {
  std::lock_guard<std::mutex> lock(mutex_conns_);
  conns_[conn->fd()] = conn;
}

void
EventLoop::undateChannel(Channel* cha) {
  epo_->updateChannel(cha);
}

void
EventLoop::removeChannel(Channel* cha) {
  epo_->removeChannel(cha);
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

void
EventLoop::handleTimer() {
  itimerspec timeout;
  bzero(&timeout, sizeof(timeout));
  timeout.it_value.tv_sec = time_val_;
  timeout.it_value.tv_nsec = 0;
  timerfd_settime(timer_fd_, 0, &timeout, 0);
  if (loop_type_ == LoopType::MAIN_LOOP) {
  } else {
    time_t now = time(0);
    for(auto it = conns_.begin(); it != conns_.end();) {
      if (it->second->isTimeout(now, timeout_)) {
        timer_cb_(it->first);  // 从TcpServer的map中删除超时的conn
        {
          std::lock_guard<std::mutex> lock(mutex_conns_);
          it = conns_.erase(it);  // 从EventLoop的map中删除超时的conn
        }
      } else 
        ++it;
    }
  }
}

void
EventLoop::setEpollTimeOutCb(EpollTimeoutCb cb) {
  epo_timeout_cb_ = std::move(cb);
}

void
EventLoop::setTimerCb(TimerCb fn) {
  timer_cb_ = std::move(fn);
}
