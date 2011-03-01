#include "job.h"

namespace bim
{
Job::Job(ThreadPool& pool, Context& context)
  :pool_(pool),
  context_(context)
{ }
}
