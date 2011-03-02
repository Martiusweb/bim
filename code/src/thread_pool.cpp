/// Author : Paul ADENOT <paul@paul.cx>
/// License : WTFPL <http://sam.zoy.org/wtfpl/>

#include "thread_pool.h"
#include "job.h"
#include "macros.h"

namespace bim
{
  ThreadPool::ThreadPool()
    :thread_count_(-1)
    ,threads_(get_thread_count())
    ,join_requested_(false)
  {
    TEST_FAILURE(pthread_mutex_init(&block_mutex_, 0));
    TEST_FAILURE(pthread_mutex_init(&queue_mutex_, 0));
    TEST_FAILURE(pthread_cond_init(&cond_wait_, 0));

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

      #ifdef BIM_THREADPOOL_THREADS_PER_CORE
      thread_count_ *= BIM_THREADPOOL_THREADS_PER_CORE;
      #endif
    }

    return thread_count_;
  }

  ThreadPool::~ThreadPool()
  {
    join();
  }

  void ThreadPool::postJob(bim::Job* job)
  {
    // Yes, two locks here : one for the condition, which, according to the
    // manpages, shall be locked when calling pthread_cond_signal, and one for
    // mutual exclusion of the queue.
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
      bim::Job* to_perform = 0;
      for(;;)
      {
        TEST_FAILURE(pthread_mutex_lock(&queue_mutex_));
        if(! queue_.empty() )
        {
          to_perform = queue_.front();
          queue_.pop();
        }
        else
        {
          TEST_FAILURE(pthread_mutex_unlock(&queue_mutex_));
          break;
        }
        TEST_FAILURE(pthread_mutex_unlock(&queue_mutex_));

        bim::Action action = to_perform->act();
        if(action == Delete)
        {
          delete to_perform;
        }
      }
      // The thread will sleep, and will be awaken when jobs are posted.
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

