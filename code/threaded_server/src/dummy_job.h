/// Author : Paul ADENOT <paul@paul.cx>
/// License : WTFPL <http://sam.zoy.org/wtfpl/>

#ifndef DUMMY_JOB_H
#define DUMMY_JOB_H

#include "job.h"

namespace bim
{
  class ThreadPool;
  class Context;
  class DummyJob : public Job
  {
    public:
      DummyJob(ThreadPool& pool, Context& context);
      virtual bim::Action act();
  };
}

#endif
