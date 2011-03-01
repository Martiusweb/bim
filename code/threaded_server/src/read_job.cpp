#include "read_job.h"
#include "request.h"
#include "context.h"
#include "macros.h"

namespace bim
{
const size_t READ_SIZE = 512;

ReadJob::ReadJob(bim::ThreadPool& pool, int fd, Context& context)
  :Job(pool, context)
{
  request_ = new bim::Request(fd, context);
}

Action ReadJob::act()
{
  size_t rv = 0;
  char* buffer = new char[READ_SIZE];

  TEST_FAILURE((rv = read(request_->get_fd(), buffer, READ_SIZE)));

  request_->append_data(buffer);

  if(rv == READ_SIZE) // More data to read
  {
    pool_.postJob(this);
    return DontDelete;
  }
  else
  {
    std::cout << "Ok, data read, time to parse" << std::endl;
    // pool_.postJob(new ParseJob(pool_, context_, request_));
    return Delete;
  }
}

}
