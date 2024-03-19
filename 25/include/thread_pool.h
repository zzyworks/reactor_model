/***********************************************************
  > File Name: thread_pool.h
  > Author: Aklzzz
  > Mail: 2429605732@qq.com
  > Created Time: Wed 13 Mar 2024 03:42:51 PM CST
  > Modified Time:Wed 13 Mar 2024 03:42:51 PM CST
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

class ThreadPool {
public:
	using TaskFunc = std::function<void()>;

private:
	// 线程池中的线程
	std::vector<std::thread> threads_;
	// 任务队列
	std::queue<TaskFunc> task_que_;
	// 任务队列同步的互斥锁
	std::mutex mutex_;
	// 任务队列同步的条件变量
	std::condition_variable cv_;
	// 在析构函数中，把stop的值设置为true，则全部的线程将退出
	std::atomic_bool stop_;
};

#endif // THREAD_POOL_H
