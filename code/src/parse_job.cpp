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

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


#include "context.h"
#include "http_error_job.h"
#include "macros.h"
#include "parse_job.h"
#include "request.h"
#include "thread_pool.h"
#include "write_job.h"


namespace bim
{
ParseJob::ParseJob(bim::ThreadPool& pool, Context& context, Request* request)
  :Job(pool, context), request_(request)
{ }

Action ParseJob::act()
{
  // Check for protocol error
  std::cout <<"method:" << request_->get_method() << ":" << std::endl;
  if(request_->get_method() != "GET" && request_->get_method() != "POST")
  {
    if(request_->get_method() != "PUT" && request_->get_method() != "DELETE")
    {
      pool_.postJob(new HttpErrorJob(pool_, context_, request_, BAD_REQUEST_400));
      return Delete;
    }
    else
    {
      pool_.postJob(new HttpErrorJob(pool_, context_, request_,  NOT_IMPLEMENTED_501));
      return Delete;
    }
  }

  struct stat statbuf;
  int rv = stat(request_->get_path().c_str(), &statbuf);
  if(rv == -1)
  {
    switch(errno)
    {
    case EACCES:
      pool_.postJob(new HttpErrorJob(pool_, context_, request_, FORBIDDEN_403));
      return Delete;
    case ENOENT:
    case ENOTDIR:
      pool_.postJob(new HttpErrorJob(pool_, context_, request_, NOT_FOUND_404));
      return Delete;
    case ENAMETOOLONG:
      pool_.postJob(new HttpErrorJob(pool_, context_, request_, BAD_REQUEST_400));
      return Delete;
    case ENOMEM:
      pool_.postJob(new HttpErrorJob(pool_, context_, request_, INTERNAL_SERVER_ERROR_500));
      return Delete;
    }
  }
  if(S_ISDIR(statbuf.st_mode))
  {
    //pool_.postJob(new ListDirJob(pool_, context_, path));
    std::cout << "We have to list directory " << request_->get_path() << std::endl;
  }
  else
  {
    pool_.postJob(new WriteJob(pool_,
                               context_,
                               request_->get_fd(),
                               request_->get_path()));
  }

  // Check for file error (unreadable, not exist, etc.)
  return Delete;
}


}
