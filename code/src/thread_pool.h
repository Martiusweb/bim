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

#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <pthread.h>
#include <queue>
#include <sys/epoll.h>
#include <vector>

namespace bim
{
class Job;

class ThreadPool
{
  public:
    ThreadPool();
    ~ThreadPool();
    void postJob(bim::Job* job);
    size_t queue_length();

  private:
    /*
     * Return when all threads have exited.
     */
    void join();
    /**
     * Get the number of threads that are managed by this pool.
     * This function shall be adapted for each architecture, and determines the
     * number of thread to use.
     */
    int get_thread_count();
    /**
     * A thread executing that function will block, until there is jobs to
     * execute (i.e. event are posted.).
     */
    void noop_job();

    /**
     * Main thread function
     */
    static void* thread_init(void*);
    /**
     * This function, which is called by each thread when completing a job,
     * tells it which job to perform next.
     */
    void schedule();

    /**
     * This is a cache for the thread count value.
     */
    int thread_count_;
    /**
     * This store the threads id.
     */
    std::vector<pthread_t> threads_;
    /**
     * the mutex for the condition
     */   
    pthread_mutex_t block_mutex_;
    /**
     * the mutex to protect the queue from concurrent acccess
     */
    pthread_mutex_t queue_mutex_;
    /**
     * The condition on which we will block.
     */
    pthread_cond_t cond_wait_;

    std::queue<Job*> queue_;
    bool join_requested_;
};
}

#endif

