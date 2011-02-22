#include "thread_pool.h"
#include "../macros.hpp"

namespace bim
{
  ThreadPool::ThreadPool()
    :threads_(10)
  {
    TEST_FAILURE((cpu_count_ = sysconf(_SC_NPROCESSORS_CONF)) == -1);


    for(int i = 0; i < get_thread_count(); ++i)
    {
      // TODO : consider using the argument to be able to queue jobs
      TEST_FAILURE(pthread_create(&(threads_[i]),0, noop_job, 0 ));
    }
  }

  int ThreadPool::get_thread_count()
  {
    return cpu_count_;
  }

  ThreadPool::~ThreadPool()
  {
    join();
  }

  void ThreadPool::setEventQueue(epoll_event* events, const int size)
  {
  }

  void ThreadPool::join()
  {
    for(int i = 0; i < get_thread_count(); ++i)
    {
      pthread_join(threads_[i], 0);
      std::cout << "joined" << std::endl;
    }
  }


  void* ThreadPool::noop_job(void*)
  {
    std::cout << "It's alive" << std::endl;
    return 0;
  }

  }

