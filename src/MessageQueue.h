#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include <condition_variable>
#include <deque>
#include <mutex>
#include <vector>

template <class T> class MessageQueue {
public:
  void send(T &&msg) {
    std::lock_guard<std::mutex> lock(_mutex);
    _queue.push_back(std::move(msg));
    _cond.notify_one();
  }

  std::vector<T> receive() {
    std::vector<T> msg;
    std::unique_lock<std::mutex> uLock(_mutex);
    _cond.wait(uLock, [this] { return !_queue.empty(); });
    // return all new messages
    for (int i = 0; i < _queue.size(); i++) {
      msg.emplace_back(std::move(_queue.front()));
      _queue.pop_front();
    }
    return msg;
  }

private:
  std::deque<T> _queue;
  std::condition_variable _cond;
  std::mutex _mutex;
};

#endif