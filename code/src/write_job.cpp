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
#include <iostream>
#include <fcntl.h>

#include "action.h"
#include "context.h"
#include "macros.h"
#include "thread_pool.h"
#include "request.h"
#include "write_job.h"
#include "log.h"

namespace bim
{

const size_t BLOCK_SIZE = 4 * 4096;

WriteJob::WriteJob(ThreadPool& pool,
                   Context& context,
                   Client &client,
                   const std::string& path,
                   const ContentType type,
                   const HttpStatusCode code)
:Job(pool, context)
,path_(path)
,_client(client)
,buffer_content_(type)
,code_(code)
{ }

Action WriteJob::act()
{
  if(buffer_content_ == Path)
  {
    int fd_in;
    int pipe_des[2];
    fd_in = open(path_.c_str(), O_RDONLY);

    if(fd_in == -1)
    {
      std::cerr << path_ << std::endl;
      std::cerr << strerror(errno) << std::endl;
    }

    TEST_FAILURE(pipe(pipe_des));

    TEST_FAILURE(posix_fadvise(fd_in, 0,0,POSIX_FADV_SEQUENTIAL | POSIX_FADV_WILLNEED));

    int rv = 0;
    {
      do {
        rv = splice(fd_in, 0, pipe_des[1], 0, BLOCK_SIZE, SPLICE_F_MORE|SPLICE_F_MOVE);
        rv = splice(pipe_des[0], 0, _client.getDescriptor(), 0, rv, SPLICE_F_MORE|SPLICE_F_MOVE);
      } while (rv > 0); 
    }

    _client.requestProcessed();

    TEST_FAILURE(close(fd_in));
    TEST_FAILURE(close(pipe_des[0]));
    TEST_FAILURE(close(pipe_des[1]));
  }
  else
  {
    int rv = 0;
    size_t offset = 0;
    size_t to_write = path_.size();
    do {
      rv = write(_client.getDescriptor(), path_.c_str() + offset, (to_write > BLOCK_SIZE) ? BLOCK_SIZE : to_write);
      offset += rv;
      to_write -= rv;
    } while(to_write > 0 && rv > 0);

    _client.requestProcessed();
  }

  return Delete;
}

}

