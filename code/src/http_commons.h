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

#ifndef BIM_HTTP_COMMONS_H
#define BIM_HTTP_COMMONS_H

#include <string>
#include <map>

namespace bim
{
  enum HttpStatusCode
  {
    CONTINUE_100,
    SWITCHING_POTOCOLS_101,
    OK_200,
    CREATED_201,
    ACCEPTED_202,
    NON_AUTHORITATIVE_INFORMATION_203,
    NO_CONTENT_204,
    RESET_CONTENT_205,
    PARTIAL_CONTENT_206,
    MULTIPLE_CHOICE_300,
    MOVED_PERMANENTLY_301,
    FOUND_302,
    SEE_OTHER_303,
    NOT_MODIFIED_304,
    USE_PROXY_305,
    TEMPORARY_REDIRECT_307,
    BAD_REQUEST_400,
    UNAUTHORIZED_401,
    PAYMENT_REQUIRED_402,
    FORBIDDEN_403,
    NOT_FOUND_404,
    METHOD_NOT_ALLOWED_405,
    NOT_ACCEPTABLE_406,
    PROXY_AUTHENTICATION_REQUIRED_407,
    REQUEST_TIMEOUT_408,
    CONFLICT_409,
    GONE_410,
    LENGTH_REQUIRED_411,
    PRECONDITION_FAILED_412,
    REQUEST_ENTITY_TOO_LARGE_413,
    REQUEST_URI_TOO_LARGE_414,
    UNSUPPORTED_MEDIA_TYPE_415,
    REQUESTED_RANGE_NOT_SATISFIABLE_416,
    EXPECTATION_FAILED_417,
    INTERNAL_SERVER_ERROR_500,
    NOT_IMPLEMENTED_501,
    BAD_GATEWAY_502,
    SERVICE_UNAVAILABLE_503,
    GATEWAY_TIMEOUT_504,
    HTTP_VERSION_NOT_SUPPORTED_505,
    _HTTP_STATUS_CODE_SIZE
  };

  typedef std::map<std::string, std::string> HeadersMap;

  enum HttpVersion {UNKNOWN, HTTP10, HTTP11};
}

#endif
