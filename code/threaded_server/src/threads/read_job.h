#ifndef _READ_JOB_H_
#define _READ_JOB_H_

#include "job.h"

class ReadJob : public Job
{
  public:
    ReadJob(int fd);
    void act();
};

#endif
