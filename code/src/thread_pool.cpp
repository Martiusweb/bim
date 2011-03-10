/**
 * Copyright (c) 2011, Paul ADENOT & Martin Richard
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * - Neither the name of the bim nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File author : Paul ADENOT <paul@paul.cx>
 *
 **/

#include <assert.h>

#include "thread_pool.h"
#include "job.h"
#include "macros.h"

#include <errno.h>

namespace bim
{
  ThreadPool::ThreadPool(int threads_per_core)
    :threads_per_core_(threads_per_core)
    ,thread_count_(-1)
    ,threads_(get_thread_count())
    ,join_requested_(false)
  {
  }

bool ThreadPool::init() {
    if(pthread_mutex_init(&block_mutex_, 0) != 0) {
        return false;
    }

    if(pthread_mutex_init(&queue_mutex_, 0)) {
        pthread_mutex_destroy(&block_mutex_);
        return false;
    }

    if(pthread_cond_init(&cond_wait_, 0)) {
        pthread_mutex_destroy(&block_mutex_);
        pthread_mutex_destroy(&queue_mutex_);
        return false;
    }

    for(int i = 0; i < get_thread_count(); ++i)
    {
      // TODO : consider using the argument to be able to queue jobs
      // TODO : manage errors properly 
      TEST_FAILURE(pthread_create(&(threads_[i]),0, thread_init, this ));
    }
    return true;
}

  int ThreadPool::get_thread_count()
  {
    // a value of -1 means that the number of threads should be changed
    if(thread_count_ == -1)
    {
      TEST_FAILURE((thread_count_ = sysconf(_SC_NPROCESSORS_CONF)) == -1);

      thread_count_ *= threads_per_core_;
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

    TEST_FAILURE(pthread_mutex_lock(&queue_mutex_));
    assert(queue_.empty());
    TEST_FAILURE(pthread_mutex_unlock(&queue_mutex_));
    TEST_FAILURE(pthread_cond_wait(&cond_wait_, &block_mutex_));

    TEST_FAILURE(pthread_mutex_unlock(&block_mutex_));

    if(join_requested_)
      pthread_exit(0);
  }

  size_t ThreadPool::queue_length()
  {

    TEST_FAILURE(pthread_mutex_lock(&queue_mutex_));
    size_t size = queue_.size();
    TEST_FAILURE(pthread_mutex_unlock(&queue_mutex_));
    return size;
  }
}

