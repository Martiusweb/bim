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
 * File author : Martin Richard <martius@martiusweb.net>
 *
 **/

#include "response.h"

namespace bim {

Response::Response(HttpStatusCode code): _status_code(code), _headers(), _headers_sent(false), _str_head(), _response_job(0) {
}

std::string& Response::getHeader() {
  if(_str_head.empty())
  {
    _str_head += "HTTP/1.1 ";

    // Status code
    switch(_status_code)
    {
      case CONTINUE_100:
        _str_head += "100 Continue";
        break;
      case SWITCHING_POTOCOLS_101:
        _str_head += "101 Switching Protocols";
        break;
      case OK_200:
        _str_head += "200 OK";
        break;
      case CREATED_201:
        _str_head += "201 Created";
        break;
      case ACCEPTED_202:
        _str_head += "202 Accepted";
        break;
      case NON_AUTHORITATIVE_INFORMATION_203:
        _str_head += "203 Non-Authoritative Information";
        break;
      case NO_CONTENT_204:
        _str_head += "204 No Content";
        break;
      case RESET_CONTENT_205:
					_str_head += "205 Reset Content";
        break;
      case PARTIAL_CONTENT_206:
					_str_head += "206 Partial Content";
        break;
      case MULTIPLE_CHOICE_300:
					_str_head += "300 Multiple Choices";
        break;
      case MOVED_PERMANENTLY_301:
					_str_head += "301 Moved Permanently";
        break;
      case FOUND_302:
					_str_head += "302 Found";
        break;
      case SEE_OTHER_303:
					_str_head += "303 See Other";
        break;
      case NOT_MODIFIED_304:
					_str_head += "304 Not Modified";
        break;
      case USE_PROXY_305:
					_str_head += "305 Use Proxy";
        break;
      case TEMPORARY_REDIRECT_307:
					_str_head += "307 Temporary Redirect";
        break;
      case BAD_REQUEST_400:
					_str_head += "400 Bad Request";
        break;
      case UNAUTHORIZED_401:
					_str_head += "401 Unauthorized";
        break;
      case PAYMENT_REQUIRED_402:
					_str_head += "402 Payment Required";
        break;
      case FORBIDDEN_403:
					_str_head += "403 Forbidden";
        break;
      case NOT_FOUND_404:
					_str_head += "404 Not Found";
        break;
      case METHOD_NOT_ALLOWED_405:
					_str_head += "405 Method Not Allowed";
        break;
      case NOT_ACCEPTABLE_406:
					_str_head += "406 Not Acceptable";
        break;
      case PROXY_AUTHENTICATION_REQUIRED_407:
					_str_head += "407 Proxy Authentication Required";
        break;
      case REQUEST_TIMEOUT_408:
					_str_head += "408 Request Time-out";
        break;
      case CONFLICT_409:
					_str_head += "409 Conflict";
        break;
      case GONE_410:
					_str_head += "410 Gone";
        break;
      case LENGTH_REQUIRED_411:
					_str_head += "411 Length Required";
        break;
      case PRECONDITION_FAILED_412:
					_str_head += "412 Precondition Failed";
        break;
      case REQUEST_ENTITY_TOO_LARGE_413:
					_str_head += "413 Request Entity Too Large";
        break;
      case REQUEST_URI_TOO_LARGE_414:
					_str_head += "414 Request-URI Too Large";
        break;
      case UNSUPPORTED_MEDIA_TYPE_415:
					_str_head += "415 Unsupported Media Type";
        break;
      case REQUESTED_RANGE_NOT_SATISFIABLE_416:
					_str_head += "416 Requested range not satisfiable";
        break;
      case EXPECTATION_FAILED_417:
					_str_head += "417 Expectation Failed";
        break;
      // 500 = default because it's then an error...
      case NOT_IMPLEMENTED_501:
					_str_head += "501 Not Implemented";
        break;
      case BAD_GATEWAY_502:
					_str_head += "502 Bad Gateway";
        break;
      case SERVICE_UNAVAILABLE_503:
					_str_head += "503 Service Unavailable";
        break;
      case GATEWAY_TIMEOUT_504:
					_str_head += "504 Gateway Time-out";
        break;
      case HTTP_VERSION_NOT_SUPPORTED_505:
					_str_head += "505 HTTP Version not supported";
        break;
      case INTERNAL_SERVER_ERROR_500:
      default:
					_str_head += "500 Internal Server Error";
        break;
    }
    _str_head += "\r\n";

    // Put headers
    for(HeadersMap::iterator header_it = _headers.begin(); header_it !=
        _headers.end(); ++header_it) {
      _str_head += header_it->first + ":" + header_it->second + "\r\n";
    }

    // End of headers
    _str_head += "\r\n";
  }
  return _str_head;
}

void Response::setStatusCode(HttpStatusCode code) {
  _status_code = code;
}

void Response::addHeader(std::string key, std::string value) {
  _headers[key] = value;
}

void Response::setHeaderSent() {
  _headers_sent = true;
}

void Response::setResponseJob(Job* response_job) {
  _response_job = response_job;
}

}

