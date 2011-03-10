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

#include <algorithm>
#include <sstream>

namespace bim 
{

using namespace std;

Request::Request(Client &client, Context& context)
:_headers_parsed(false)
,_http_version(UNKNOWN)
,_client(client)
,context_(context)
{
  _client.requestHandled();
}

std::string& Request::getMethod()
{
  _parse_headers();
  if(method_.empty()) {
    _parse_request_line();
  }
  return method_;
}

std::string& Request::getUrl()
{
  if(url_.empty()) {
    _parse_request_line();
  }
  return url_;
}

std::string& Request::getPath()
{
  if(path_.empty())
  {
    path_ = context_.getDocumentRoot()+getUrl();
  }
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


Request::HttpVersion Request::getHttpVersion() {

  if(_http_version == UNKNOWN) {
    _parse_request_line();
  }
  return _http_version;
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
  if(!_headers_parsed) {
    _parse_headers();
  }
  return (_headers.find(header) == _headers.end());
}

const std::string& Request::getHeader(const std::string& header) {
  if(!_headers_parsed) {
    _parse_headers();
  }
  return _headers[header];
}

const Request::HeadersMap& Request::getHeaders() {
  if(!_headers_parsed) {
    _parse_headers();
  }
  return _headers;
}

void Request::_parse_request_line() {
  // Parse the method
  size_t end_method = raw_.find_first_of(' ');
  if(end_method != std::string::npos)
  {
    method_ = raw_.substr(0,end_method);

    // Parse the URL
    size_t end_url = raw_.find_first_of(' ', end_method+1);

    if(end_url != string::npos) {
      url_ = raw_.substr(end_method+1, end_url - end_method - 1);

      // Parse http version
      size_t begin_version = raw_.find_first_of('/', end_url+1);
      _http_version = raw_.substr(begin_version+1, 3) == "1.0" ? HTTP10 : HTTP11;
    }
  }
}

void Request::_parse_headers() {
  std::stringstream request(raw_);
  std::string current;
  size_t separator;
  std::string key,value;

  // Skip request line
  getline(request, current);

  while(!request.eof()) {
    getline(request, current);

    if(!current.empty() && (separator = current.find_first_of(':')) !=
        std::string::npos)
    {
      key = current.substr(0, separator);
      value = current.substr(separator+1, current.find_first_of('\r')-separator-1);
      std::transform(key.begin(), key.end(), key.begin(), ::tolower);
      _headers[key] = value;
    }
  }
  _headers_parsed = true;
}

}

