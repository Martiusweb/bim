#include "thread_pool.h"
#include "job.h"
#include "../macros.hpp"

namespace bim
{
  ThreadPool::ThreadPool()
    :thread_count_(-1)
    ,threads_(get_thread_count())
    ,block_mutex_(PTHREAD_MUTEX_INITIALIZER)
    ,queue_mutex_(PTHREAD_MUTEX_INITIALIZER)
    ,cond_wait_(PTHREAD_COND_INITIALIZER)
    ,join_requested_(false)
  {
    for(int i = 0; i < get_thread_count(); ++i)
    {
      // TODO : consider using the argument to be able to queue jobs
      TEST_FAILURE(pthread_create(&(threads_[i]),0, thread_init, this ));
    }
  }

  int ThreadPool::get_thread_count()
  {
    // a value of -1 means that the number of threads should be changed
    if(thread_count_ == -1)
    {
      TEST_FAILURE((thread_count_ = sysconf(_SC_NPROCESSORS_CONF)) == -1);
      thread_count_ *= 20;
    }

    return thread_count_;
  }

  ThreadPool::~ThreadPool()
  {
    join();
  }

  void ThreadPool::postJob(bim::Job* job)
  {
    TEST_FAILURE(pthread_mutex_lock(&block_mutex_));
    TEST_FAILURE(pthread_mutex_lock(&queue_mutex_));
    queue_.push(job);
    TEST_FAILURE(pthread_mutex_unlock(&queue_mutex_));
    TEST_FAILURE(pthread_cond_signal(&cond_wait_));
    TEST_FAILURE(pthread_mutex_unlock(&block_mutex_));
  }

  void ThreadPool::join()
  {
    for(int i = 0; i < get_thread_count(); ++i)
    {
      join_requested_ = true;
     
      // unblock all thread, if they were on no-op
      TEST_FAILURE(pthread_cond_broadcast(&cond_wait_));

      pthread_join(threads_[i], 0);
    }
  }

  void* ThreadPool::thread_init(void* instance)
  {
    reinterpret_cast<ThreadPool*>(instance)->schedule();
    return 0;
  }
    
  void ThreadPool::schedule()
  {
    for(;;)
    {
    // This locking is flawed, it crashes after a while. Presumably, race
    // condition on the while we should use two mutex here.
    while( ! queue_.empty())
      {
        TEST_FAILURE(pthread_mutex_lock(&queue_mutex_));
        bim::Job* to_perform = queue_.front();
        queue_.pop();
        TEST_FAILURE(pthread_mutex_unlock(&queue_mutex_));
        if(to_perform != 0)
        {
          to_perform->act();
          delete to_perform;
        }
        else
        {
          std::cerr << "########### race condition, about to segfault" << std::endl;
          std::cerr << "########### queue size : " << queue_.size() << std::endl;
        }
      }
      noop_job();
    }
  }

  void ThreadPool::noop_job()
  {
    TEST_FAILURE(pthread_mutex_lock(&block_mutex_));

    TEST_FAILURE(pthread_cond_wait(&cond_wait_, &block_mutex_));

    TEST_FAILURE(pthread_mutex_unlock(&block_mutex_));

    if(join_requested_)
      pthread_exit(0);
  }

  size_t ThreadPool::queue_length()
  {
    return queue_.size();
  }
}

