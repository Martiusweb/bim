#ifndef _JOB_H_
#define _JOB_H_

class Job
{
  public:
    /**
     * @brief This method actually do the job. It shall be called by a worker
     * thread.
     */
    void act() = 0;
};

#endif
