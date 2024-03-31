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

private:
  std::unique_ptr<Epoll> epo_;
	// epoll_wait()超时的回调函数
	EpollTimeoutCb epo_timeout_cb_;
};

#endif // EVENT_LOOP_H
