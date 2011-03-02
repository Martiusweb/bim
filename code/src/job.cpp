/// Author : Paul ADENOT <paul@paul.cx>
/// License : WTFPL <http://sam.zoy.org/wtfpl/>

#include "job.h"

namespace bim
{
Job::Job(ThreadPool& pool, Context& context)
  :pool_(pool),
  context_(context)
{ }
}
