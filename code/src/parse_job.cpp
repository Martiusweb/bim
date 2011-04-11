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
        HttpErrorJob ejob(pool_, context_, request_);
        ejob.act();
        return Delete;
      }
      else
      {
        request_.getClient().requestParsed();
        request_.getResponse().setStatusCode(NOT_IMPLEMENTED_501);
        HttpErrorJob ejob(pool_, context_, request_);
        ejob.act();
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
        case ENOENT:
        case ENOTDIR:
          request_.getResponse().setStatusCode(NOT_FOUND_404);
        case ENAMETOOLONG:
          request_.getResponse().setStatusCode(BAD_REQUEST_400);
        case ENOMEM:
          request_.getResponse().setStatusCode(INTERNAL_SERVER_ERROR_500);
        default:
          TEST_FAILURE(rv);
          return Delete;
      }

      request_.getClient().requestParsed();
      HttpErrorJob ej(pool_, context_, request_);
      ej.act();
      return Delete;
    }
    if(S_ISDIR(statbuf.st_mode))
    {
      ListJob lj(pool_, context_, request_);
      lj.act();
      request_.getClient().requestParsed();
    }
    else
    {
      WriteJob wj(pool_, context_, request_, request_.getPath(), WriteJob::Path, (size_t) statbuf.st_size);
      wj.act();
      request_.getClient().requestParsed();
    }

    // Check for file error (unreadable, not exist, etc.)
    return Delete;
  }

}

