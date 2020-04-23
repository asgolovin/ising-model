#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include <deque>
#include <mutex>
#include <condition_variable>

template <class T>
class MessageQueue
{
public:
  void send(T &&msg);
  T receive();

private:
  std::deque<double[2]> _queue;
  std::condition_variable _cond;
  std::mutex _mutex;
};

#endif