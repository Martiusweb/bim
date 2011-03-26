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

#include <string>
#include <iostream>
#include <cstring>
#include "http11_parser_mongrel.h"
#include "http11_request.h"

#ifndef PARSER_H
#define PARSER_H

namespace bim {
class http11_parser
{
  public:
    http11_parser(const char* buffer);

    void execute();

    http11_request* finished();

  private:
    void http_field(void *data, const char *field, size_t flen, const char *value, size_t vlen);
    static void http_field_(void* instance, void *data, const char *field, size_t flen, const char *value, size_t vlen);

    void request_method(void *data, const char *at, size_t length);
    void request_uri(void *data, const char *at, size_t length);
    void fragment(void *data, const char *at, size_t length);
    void request_path(void *data, const char *at, size_t length);
    void query_string(void *data, const char *at, size_t length);
    void http_version(void *data, const char *at, size_t length);
    void header_done(void *data, const char *at, size_t length);

    static void request_method_(void* instance, void *data, const char *at, size_t length);
    static void request_uri_(void* instance, void *data, const char *at, size_t length);
    static void fragment_(void* instance, void *data, const char *at, size_t length);
    static void request_path_(void* instance, void *data, const char *at, size_t length);
    static void query_string_(void* instance, void *data, const char *at, size_t length);
    static void http_version_(void* instance, void *data, const char *at, size_t length);
    static void header_done_(void* instance, void *data, const char *at, size_t length);

    http_parser* parser_;
    const char* buffer_;
    http11_request* request_;
};

}

#endif
