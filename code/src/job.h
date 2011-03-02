/// Author : Paul ADENOT <paul@paul.cx>
/// License : WTFPL <http://sam.zoy.org/wtfpl/>

#ifndef _JOB_H_
#define _JOB_H_

#include "thread_pool.h"
#include "context.h"
#include "action.h"

namespace bim
{
class Job
{
  public:
    Job(ThreadPool& pool, Context& context);
    /**
     * @brief This method actually do the job. It shall be called by a worker
     * thread.
     */
    virtual bim::Action act() = 0;

  protected:
    bim::ThreadPool& pool_;
    bim::Context& context_;
};
}

#endif
