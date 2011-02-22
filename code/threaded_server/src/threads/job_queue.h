#ifndef _JOB_QUEUE_H_
#define _JOB_QUEUE_H_

#include "job.h"

namespace bim
{

  class JobQueue
  {
    public:
      /**
       * @brief Put a job at the end of the queue
       *
       * @param job A pointer (acquired) to the job.
       */
      void enqueue(Job* job);

      /**
       * @brief Pop the first job of the queue
       *
       * @return  The firt job of the queue, 0 if the queue is empty
       */
      Job* front();
    private:
      std::queue<Job*> jobs_;
  };

}

#endif

