#ifndef _JOB_H_
#define _JOB_H_

#include "thread_pool.h"

namespace bim
{
class Job
{
  public:
    Job(ThreadPool& pool);
    /**
     * @brief This method actually do the job. It shall be called by a worker
     * thread.
     */
    virtual void act() = 0;

    ThreadPool& pool_;
};
}

#endif
