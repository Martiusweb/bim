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
#include <cstring>

#include "context.h"
#include "macros.h"
#include "parse_job.h"
#include "read_job.h"
#include "request.h"


namespace bim
{
const int READ_SIZE = 512;

ReadJob::ReadJob(bim::ThreadPool& pool, Client &client, Context& context)
  :Job(pool, context), _client(client)
{
  request_ = new bim::Request(client, context);
}

Action ReadJob::act()
{
  int rv = 0;
  char* buffer = new char[READ_SIZE];
  char tmp;
  char* str_request = buffer;

  // valgrind was complaining about unitialiazed memory.
  // this is maybe not necessary
  TEST_FAILURE(memset(buffer, READ_SIZE, 1));

  rv = read(request_->getClient().getDescriptor(), buffer, READ_SIZE);

  // Typo on bitmask
  if(rv == -1 && (errno & (EAGAIN | EWOULDBLOCK)))
  {
    pool_.postJob(this);
    delete[] buffer;
    return DontDelete;
  }

  // rv was readsize, here
  for(int i = 0; i < rv; ++i)
  {
    if(buffer[i] == '\r' && buffer[i+1] == '\n'
        && buffer[i+2] == '\r' && buffer[i+3] == '\n')
    {
      // End of the request
      tmp = buffer[i+4];
      buffer[i+4] = '\0';
      request_->appendData(str_request);
      str_request = buffer+4;
      str_request[0] = tmp;

      // Not the end we have got something else to read
      if(str_request[0] != '\0') {
        pool_.postJob(new ParseJob(pool_, context_, *request_));
        // Dealing with a new request.
        request_ = new Request(_client, context_);
      }
    }
  }

  if(rv == READ_SIZE) // More data to read
  {
    pool_.postJob(this);
    delete[] buffer;
    return DontDelete;
  }
  else if(rv == 0) // client closed connection
  {
    delete[] buffer;
    return Delete;
  }
  else
  {
    pool_.postJob(new ParseJob(pool_, context_, *request_));
    _client.requestsRead();
    delete[] buffer;
    return Delete;
  }
}

}
