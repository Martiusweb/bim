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
#include "list_job.h"
#include "log.h"
#include "macros.h"
#include "parse_job.h"
#include "request.h"
#include "thread_pool.h"
#include "write_job.h"


namespace bim
{
ParseJob::ParseJob(bim::ThreadPool& pool, Context& context, Request& request)
  :Job(pool, context), request_(request)
{ }

Action ParseJob::act()
{
  access_log(request_.get_request_line());

  if(request_.getMethod() != "GET" && request_.getMethod() != "POST")
  {
    if(request_.getMethod() != "PUT" && request_.getMethod() != "DELETE")
    {
      request_.getClient().requestParsed();
      request_.getResponse().setStatusCode(BAD_REQUEST_400);
      pool_.postJob(new HttpErrorJob(pool_, context_, request_));
      return Delete;
    }
    else
    {
      request_.getClient().requestParsed();
      request_.getResponse().setStatusCode(NOT_IMPLEMENTED_501);
      pool_.postJob(new HttpErrorJob(pool_, context_, request_));
      return Delete;
    }
  }

  struct stat statbuf;
  int rv = stat(request_.getPath().c_str(), &statbuf);
  if(rv == -1)
  {
    switch(errno)
    {
    case EACCES:
      request_.getResponse().setStatusCode(FORBIDDEN_403);
      pool_.postJob(new HttpErrorJob(pool_, context_, request_));
      request_.getClient().requestParsed();
      return Delete;
    case ENOENT:
    case ENOTDIR:
      request_.getResponse().setStatusCode(NOT_FOUND_404);
      pool_.postJob(new HttpErrorJob(pool_, context_, request_));
      request_.getClient().requestParsed();
      return Delete;
    case ENAMETOOLONG:
      request_.getResponse().setStatusCode(BAD_REQUEST_400);
      pool_.postJob(new HttpErrorJob(pool_, context_, request_));
      request_.getClient().requestParsed();
      return Delete;
    case ENOMEM:
      request_.getResponse().setStatusCode(INTERNAL_SERVER_ERROR_500);
      pool_.postJob(new HttpErrorJob(pool_, context_, request_));
      request_.getClient().requestParsed();
      return Delete;
    default:
      TEST_FAILURE(rv);
      return Delete;
    }
  }
  if(S_ISDIR(statbuf.st_mode))
  {
    pool_.postJob(new ListJob(pool_, context_, request_));
    request_.getClient().requestParsed();
  }
  else
  {
    pool_.postJob(new WriteJob(pool_,
                               context_,
                               request_,
                               request_.getPath()));
    request_.getClient().requestParsed();
  }

  // Check for file error (unreadable, not exist, etc.)
  return Delete;
}

}

