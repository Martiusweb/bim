/**
 * Copyright (c) 2011, Paul ADENOT & Martin Richard
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * - Neither the name of the bim nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
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

#include "http11_parser.h"
#include <cassert>

namespace bim {

  http11_parser::http11_parser(const char* buffer)
:buffer_(buffer)
{
  parser_ = new http_parser;
  request_ = new http11_request;

  http_parser_init(parser_);

  parser_->instance = reinterpret_cast<void*>(this);

  parser_->http_field = http_field_;
  parser_->request_method =request_method_;
  parser_->request_uri = request_uri_;
  parser_->fragment = fragment_;
  parser_->request_path = fragment_;
  parser_->query_string = query_string_;
  parser_->http_version = http_version_;
  parser_->header_done = header_done_;
}

void http11_parser::execute()
{
  http_parser_execute(parser_, buffer_, strlen(buffer_), 0);
}


http11_request* http11_parser::finished()
{
  int state = http_parser_finish(parser_);

  switch(state)
  {
    case 0:
    case 1:
      return request_;
    case -1:
      return 0;
    default:
      assert(false && "wtf ?");
  }
}

void http11_parser::http_field(void* /* data */, const char *field, size_t flen, const char *value, size_t vlen)
{
  std::string key(field, flen);
  std::string val(value, vlen);
  request_->header_fields[key] = val;
}

void http11_parser::http_field_(void* instance, void*  data, const char *field, size_t flen, const char *value, size_t vlen)
{
  reinterpret_cast<http11_parser*>(instance)->http_field(data, field, flen, value, vlen);
}


void http11_parser::request_method(void* /* data */, const char *at, size_t length)
{
  request_->request_method=std::string(at, length);
}

void http11_parser::request_uri(void* /* data */, const char *at, size_t length)
{
  request_->request_uri = std::string(at, length);
}

void http11_parser::fragment(void* /* data */, const char *at, size_t length)
{
  request_->fragment = std::string(at, length);
}

void http11_parser::request_path(void* /* data */, const char *at, size_t length)
{
  request_->request_path = std::string(at, length);
}

void http11_parser::query_string(void* /* data */, const char *at, size_t length)
{
  request_->query_string = std::string(at, length);
}

void http11_parser::http_version(void* /* data */, const char *at, size_t length)
{
  std::string str(at,length);

  if(str == "HTTP/1.1")
    request_->http_version = HTTP11;
  else if (str == "HTTP/1.0")
    request_->http_version = HTTP10;
  else
    request_->http_version = UNKNOWN;

  request_->http_version;
}

void http11_parser::header_done(void* /* data */, const char *at, size_t length)
{
  request_->header_done = std::string(at, length);
}

void http11_parser::request_method_(void* instance, void *data, const char *at, size_t length)
{
  reinterpret_cast<http11_parser*>(instance)->request_method(data, at, length);
}

void http11_parser::request_uri_(void* instance, void *data, const char *at, size_t length)
{
  reinterpret_cast<http11_parser*>(instance)->request_uri(data, at, length);
}

void http11_parser::fragment_(void* instance, void *data, const char *at, size_t length)
{
  reinterpret_cast<http11_parser*>(instance)->fragment(data, at, length);
}

void http11_parser::request_path_(void* instance, void *data, const char *at, size_t length)
{
  reinterpret_cast<http11_parser*>(instance)->request_path(data, at, length);
}

void http11_parser::query_string_(void* instance, void *data, const char *at, size_t length)
{
  reinterpret_cast<http11_parser*>(instance)->query_string(data, at, length);
}

void http11_parser::http_version_(void* instance, void *data, const char *at, size_t length)
{
  reinterpret_cast<http11_parser*>(instance)->http_version(data, at, length);
}

void http11_parser::header_done_(void* instance, void *data, const char *at, size_t length)
{
  reinterpret_cast<http11_parser*>(instance)->header_done(data, at, length);
}
}
