/**
 * Copyright (c) 2011, Paul ADENOT & Martin Richard
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * - Neither the name of the bim nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File author : Paul ADENOT <paul@paul.cx>
 *
 **/

#include <unistd.h>

#include "context.h"
#include "macros.h"
#include "read_job.h"
#include "request.h"


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

  rv = read(request_->get_fd(), buffer, READ_SIZE);

  if(rv == -1 && errno == EAGAIN | EWOULDBLOCK)
  {
    pool_.postJob(this);
    return DontDelete;
  }

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
