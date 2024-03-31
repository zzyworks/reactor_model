/**
 * @file   event_loop.h
 * @author Aklzzz(csdn)
 * @date   2024/03/04 18:57:15
 * @note   
 *  事件循环类
*/

#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include <functional>
#include <unistd.h>
#include <sys/syscall.h>

#include "my_epoll.h"
#include "channel.h"
#include "noncopyable.h"

class Channel;
class Epoll;

class EventLoop : Noncopyable {
public:
  using EpollTimeoutCb = std::function<void(EventLoop*)>; 

  EventLoop();

  ~EventLoop() = default;

  void run();

  /// @brief 把channel添加/更新到红黑树上
  void undateChannel(Channel* cha);

  // 从红黑树上删除channel
  void removeChannel(Channel* cha);

	void setEpollTimeOutCb(EpollTimeoutCb cb);

  // 判断当前线程是否为事件循环线程
  bool isInLoopThread();

private:
  std::unique_ptr<Epoll> epo_;
	// epoll_wait()超时的回调函数
	EpollTimeoutCb epo_timeout_cb_;
  pid_t thread_id_; // 事件循环所在线程的id
};

#endif // EVENT_LOOP_H
