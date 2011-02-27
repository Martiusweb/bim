#include <iostream>
#include "thread_pool.h"
#include "dummy_job.h"
#include <assert.h>
#include <vector>
#include <cstdio>
#include <stdlib.h>

using namespace bim;
using namespace std;

int main()
{
  ThreadPool pool; 
  std::vector<Job*> jobs(10);

  for(unsigned i = 0; i < jobs.size(); i++)
  {
    jobs[i] = new DummyJob(pool);
  }

  srand(time(0));

  // Since each dummy job posts another dummy job in the queue, this program
  // shall run indefinitely, and the queue length shall increase.
  for(unsigned i = 0; i < jobs.size(); i++)
  {
    pool.postJob(jobs[i]);
    usleep(random() % 1000);
    cout << "Queue length : " << pool.queue_length() << endl;
  }

  getchar();

  return 0;
}
