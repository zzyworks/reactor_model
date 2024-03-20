#include "thread_pool.h"

ThreadPool::ThreadPool(std::size_t thread_n) : stop_(false) {
  this->init(thread_n);
}

ThreadPool::~ThreadPool() {
  stop_ = true;
  cv_.notify_all();  // 唤醒全部线程
  for (std::thread& thd : threads_)
    thd.join();
}

void
ThreadPool::push(Task task) {
  {
    std::lock_guard<std::mutex> lock(mutex_);
    task_que_.push(std::move(task));
  }
  cv_.notify_one();
}

void
ThreadPool::init(std::size_t thread_n) {
  for (std::size_t i = 0; i != thread_n; ++i) {
    threads_.emplace_back([this](void) {
      printf("create_thread(%ld).\n", syscall(SYS_gettid));
      while (!stop_) {
        Task task;
        {
          std::unique_lock<std::mutex> lock(mutex_);
          // 等待生产者的条件变量,如果谓词返回false阻塞（不满足条件变量就阻塞）
          cv_.wait(lock,
                   [this](void) { return ((stop_) || (!task_que_.empty())); });
          // 在线程池停止之前，如果队列中还有任务，执行完再退出
          if ((stop_) && (task_que_.empty()))
            return;
          task = std::move(task_que_.front());
          task_que_.pop();
        }
        printf("thread is %ld.\n", syscall(SYS_gettid));
        task();
      }
    });
  }
}
