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

