#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include <condition_variable>
#include <deque>
#include <mutex>

template <class T> class MessageQueue {
public:
  void send(T &&msg) {
    std::lock_guard<std::mutex> lock(_mutex);
    _queue.push_back(std::move(msg));
    _cond.notify_one();
  }

  T receive() {
    std::unique_lock<std::mutex> uLock(_mutex);
    _cond.wait(uLock, [this] { return !_queue.empty(); });
    // use FIFO order
    T msg = std::move(_queue.front());
    _queue.pop_front();
    return msg;
  }

private:
  std::deque<T> _queue;
  std::condition_variable _cond;
  std::mutex _mutex;
};

#endif