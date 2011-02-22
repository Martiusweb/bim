#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <sys/epoll.h>
#include <vector>
#include <pthread.h>

namespace bim
{

class ThreadPool
{
  public:
    ThreadPool();
    ~ThreadPool();
    void setEventQueue(epoll_event* events, const int size);

  private:
    void join();
    int get_thread_count();
    static void* noop_job(void *);

    int cpu_count_;
    std::vector<pthread_t> threads_;
};
}

#endif

