#ifndef _READ_JOB_H_
#define _READ_JOB_H_

#include "job.h"

namespace bim
{
class Context;
class Request;
class ReadJob : public Job
{
  public:
       /**
        * @brief This constructor is used when constructing this job  for the first time
        *
        * @param pool The thread pool, to be able to post jobs
        * @param fd The file descriptor, to read data
        * @param context The context, containing the document root
        */
    ReadJob(bim::ThreadPool& pool, int fd, Context& context);
    Action act();
  private:
    Request* request_;
};
}

#endif
