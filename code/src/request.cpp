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

#include "request.h"
#include "context.h"
#include "assert.h"
#include <iostream>

namespace bim 
{

using namespace std;

Request::Request(const int fd, Context& context)
:fd_(fd)
,context_(context)
{}

std::string& Request::get_method()
{
  if(method_.empty())
  {
    size_t pos = raw_.find_first_of(' ');

    if(pos != std::string::npos)
    {
      method_ = raw_.substr(0,pos);
    }
  }
  return method_;
}

std::string& Request::get_url()
{
  if(url_.empty())
  {
    size_t begin_url = raw_.find_first_of(' ');
    size_t end_url = raw_.find_first_of(' ', begin_url+1);

    if(begin_url != string::npos && end_url != string::npos)
    {
      url_ = raw_.substr(begin_url+1, end_url - begin_url - 1); 
    }
  }
  return url_;
}

std::string& Request::get_path()
{
  if(path_.empty())
  {
    std::cout << "url" << get_url() <<std::endl;
    path_ = context_.get_document_root()+get_url();
  }
  return path_;
}


int Request::get_fd()
{
  return fd_;
}

void Request::append_data(const char* data)
{
  raw_ += data;
}

const std::string& Request::get_raw() const
{
  return raw_;
}

}

