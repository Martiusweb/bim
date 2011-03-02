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

#ifndef _REQUEST_H_
#define _REQUEST_H_

#include <string>

namespace bim 
{
  class Context;
class Request
{
  public:
    /**
     * @brief Constructor for an HTTP request
     *
     * @param fd the file descriptor of the client(s socket
     * @param context The context (document_root, etc.)
     */
    Request(const int fd, Context& context);
    /**
     * @brief Get the method for the request
     *
     * @return empty in case of error, the method otherwise
     */
    std::string& get_method();
    /**
     * @brief Get the url of this request
     *
     * @return empty in case of error, the url otherwise
     */
    std::string& get_url();
    /**
     * @brief Get the path for this request
     * @todo get document_root from context
     * @return empty in case of error, the path otherwise.
     */
    std::string& get_path();

    /**
     * Append data to the internal buffer.
     */
    void append_data(const char* data);

    /**
     * @brief Get the fd back.
     *
     * @return the fd for this request.
     */
    int get_fd();

  private:
    /* Change that to ranges ? */
    std::string path_;
    std::string url_;
    std::string method_;
    std::string raw_;
    int fd_;
    Context& context_;
};
}

#endif
