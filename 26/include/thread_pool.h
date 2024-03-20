/***********************************************************
  > File Name: thread_pool.h
  > Author: Aklzzz
  > Mail: 2429605732@qq.com
  > Created Time: Wed 20 Mar 2024 01:05:23 PM CST
  > Modified Time:Wed 20 Mar 2024 01:05:23 PM CST
 *******************************************************/

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <unistd.h> // for syscall
#include <sys/syscall.h> // for SYS_gettid


class ThreadPool {
public:
  using Task = std::function<void()>;

  // 启动thread_n个线程
  ThreadPool(std::size_t thread_n);

  ~ThreadPool();

  void push(Task task);

private:
  void init(std::size_t thread_n);

  std::vector<std::thread> threads_; // 线程池中的线程
  std::queue<Task> task_que_; // 任务队列
  std::mutex mutex_; // 任务队列同步的互斥锁
  std::condition_variable cv_; // 任务队列同步的条件变量
  std::atomic_bool stop_; // 如果stop_=true，则线程池停止工作
};

#endif // THREAD_POOL_H
