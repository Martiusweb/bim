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


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <cassert>
#include "http11_common_mongrel.h"
#include "http11_parser_mongrel.h"
#include "http11_parser.h"
#include "http11_request.h"

const char* httpreq = "GET /static/index.html?BABA=boubou&bi=bo#plop HTTP/1.1\r\n"
                      "Host: localhost:8080\r\n"
                      "User-Agent: Mozilla/5.0 (X11; Linux i686; rv:2.0b13pre) Gecko/20110226 Firefox/4.0b13pre\r\n"
                      "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
                      "Accept-Language: en-us,en;q=0.5\r\n"
                      "Accept-Encoding: gzip, deflate\r\n"
                      "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n"
                      "Keep-Alive: 115\r\n"
                      "Connection: keep-alive\r\n\r\n"
                      "nmljkdmlfjfsjgsljkfdgmljdsfgmjksdlfjgsjfdgjdsflkgs";


int main()
{
  http11_parser a(httpreq);
  a.execute();
  http11_request* result = a.finished();

  std::cout << ":" << result->request_method << ":" << std::endl;
  std::cout << ":" << result->request_uri    << ":" << std::endl;
  std::cout << ":" << result->fragment       << ":" << std::endl;
  std::cout << ":" << result->request_path   << ":" << std::endl;
  std::cout << ":" << result->query_string   << ":" << std::endl;
  std::cout << ":" << result->http_version   << ":" << std::endl;
  std::cout << ":" << result->header_done    << ":" << std::endl;

	assert(result->request_method == "GET");
	assert(result->request_uri == "/static/index.html?BABA=boubou&bi=bo");
	assert(result->fragment == "plop");
  assert(result->request_path == "");
	assert(result->query_string == "BABA=boubou&bi=bo");
	assert(result->http_version == "HTTP/1.1");
	assert(result->header_done == "nmljkdmlfjfsjgsljkfdgmljdsfgmjksdlfjgsjfdgjdsflkgs");

  assert(result->header_fields["Host"] == "localhost:8080");
  assert(result->header_fields["User-Agent"] == "Mozilla/5.0 (X11; Linux i686; rv:2.0b13pre) Gecko/20110226 Firefox/4.0b13pre");
  assert(result->header_fields["Accept"] == "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
  assert(result->header_fields["Accept-Language"] == "en-us,en;q=0.5");
  assert(result->header_fields["Accept-Encoding"] == "gzip, deflate");
  assert(result->header_fields["Accept-Charset"] == "ISO-8859-1,utf-8;q=0.7,*;q=0.7");
  assert(result->header_fields["Keep-Alive"] == "115");
  assert(result->header_fields["Connection"] == "keep-alive");

  return 0;
}
