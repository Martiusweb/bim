#ifndef DUMMY_JOB_H
#define DUMMY_JOB_H

#include "job.h"

namespace bim
{
  class DummyJob : public Job
  {
    public:
      DummyJob(ThreadPool& pool);
        virtual void act();
  };
}

#endif
