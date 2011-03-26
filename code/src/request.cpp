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
#include "parser/http11_parser.h"

#include <algorithm>
#include <assert.h>
#include <sstream>

namespace bim 
{

using namespace std;

Request::Request(Client &client, Context& context)
:request_(0)
,_client(client)
,context_(context)
,_response()
{
  _client.requestHandled(this);
}

Request::~Request()
{
  delete request_;
}

std::string& Request::getMethod()
{
  parse();
  return request_->request_method;
}

std::string& Request::getUrl()
{
  parse();
  return request_->request_uri;
}

std::string& Request::getPath()
{
  parse();
  path_ = context_.getDocumentRoot()+getUrl();
  return path_;
}

int Request::getFd() const
{
  return _client.getDescriptor();
}

void Request::appendData(const char* data)
{
  raw_ += data;
}

const std::string& Request::get_raw() const
{
  return raw_;
}

const std::string& Request::get_request_line()
{
  if(request_line_.empty())
  {
    size_t eol = raw_.find('\r');
    if(eol != string::npos)
    {
      request_line_ = raw_.substr(0, eol);
    }
  }
  return request_line_;
}


HttpVersion Request::getHttpVersion() {
  parse();
  return request_->http_version;
}

// TODO do it better ?
static const std::string const_headerKeepAlive = "keep-alive";
static const std::string const_headerKeepAliveClose = "close";
bool Request::keepAlive() {
  if(!headerExists(const_headerKeepAlive)) {
    return (getHttpVersion() == HTTP11);
  }

  bool result(true);
  std::string keepAlive = getHeader(const_headerKeepAlive);
  std::transform(keepAlive.begin(), keepAlive.end(), keepAlive.begin(), ::tolower);
  if(getHttpVersion() == HTTP11 && keepAlive != const_headerKeepAliveClose) {
    result = false;
  }
  else if(getHttpVersion() == HTTP10 && keepAlive != const_headerKeepAlive) {
    result = false;
  }
  return result;
}

bool Request::headerExists(const std::string& header) {
  parse();
  return (request_->header_fields.find(header) == request_->header_fields.end());
}

const std::string& Request::getHeader(const std::string& header) {
  parse();
  return request_->header_fields[header];
}

const HeadersMap& Request::getHeaders() {
  parse();
  return request_->header_fields;
}

void Request::parse()
{
  if( ! request_ ) {
    http11_parser parser(raw_.c_str());
    parser.execute();
    request_ = parser.finished();
  }
}

}

