#include <iostream>
#include <unistd.h>
#include <stdlib.h>

#include "dummy_job.h"
#include "thread_pool.h"
#include "context.h"

namespace bim
{
  DummyJob::DummyJob(ThreadPool& pool, Context& context)
    :Job(pool, context)
  { }

  bim::Action DummyJob::act()
  {
      std::cout << "[" << pthread_self() <<"] I'm doing stuff." << std::endl;
      usleep(random() % 1000); //sleep between 0 and 1 second
      std::cout << "[" << pthread_self() <<"] I'm done." << std::endl;
      //Job* job = new DummyJob(pool_);
      pool_.postJob(this);
      std::cout << "[" << pthread_self() 
                <<"] queue length : " 
                << pool_.queue_length() << std::endl;

      return DontDelete;
  }
}

