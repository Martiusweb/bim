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

#ifndef BIM_REQUEST_H_
#define BIM_REQUEST_H_

#include <string>

#include "client.h"
#include "http_commons.h"
#include "response.h"

namespace bim
{
class Context;
class Request
{
  public:
    /**
     * @brief Constructor for an HTTP request
     *
     * @param client The client object
     * @param context The context (document_root, etc.)
     */
    Request(Client &client, Context& context);

    /**
     * @brief Get the method for the request
     *
     * @return empty in case of error, the method otherwise
     */
    std::string& getMethod();

    /**
     * @brief Get the url of this request
     *
     * @return empty in case of error, the url otherwise
     */
    std::string& getUrl();

    /**
     * @brief Get the path for this request
     * @todo get document_root from context
     * @return empty in case of error, the path otherwise.
     */
    std::string& getPath();

    const std::string& get_raw() const;

    /**
     * Append data to the internal buffer.
     */
    void appendData(const char* data);

    /**
     * @brief Get the fd back.
     * use getClient().getDescriptor() instead.
     *
     * @deprecated
     * @return the fd for this request.
     */
    int getFd() const;

    /**
     * @brief returns the client who initiated the request.
     * 
     * @return the client of the request
     */
    inline Client& getClient() const {
      return _client;
    }

    /**
     * @brief returns the http version (1.0 or 1.1).
     * @return version code
     */
    HttpVersion getHttpVersion();

    /**
     * @brief Get the request line, without CRLF
     *
     * @return the request line.
     */
    const std::string& get_request_line();

    /**
     * @brief checks if an header key exists for this request
     *
     * Be aware that headers are case sensitives inside the request object.
     * for convenience, headers are all lowercase.
     *
     * @return true if the request exists
     */
    bool headerExists(const std::string& header);

    /**
     * @brief Gets the value of the given header.
     *
     * You must check if the header key exists with headerExists() before.
     * Be aware that headers are case sensitives inside the request object.
     * for convenience, headers are all lowercase.
     *
     * @return the header value
     */
    const std::string& getHeader(const std::string& header);

    /**
     * @brief get a maps of headers.
     *
     * Be aware that headers are case sensitives inside the request object.
     * for convenience, headers are all lowercase.
     *
     * @return the request headers.
     */
    const HeadersMap& getHeaders();

    inline Response& getResponse() {
      return _response;
    }

    /**
     * @brief checks if the request asks or not for a persistent connection.
     *
     * Checks accoring to the headers and the protocol version.
     * @return true if the connection should be persistent
     */
    bool keepAlive();

  private:
    /**
     * @brief retrieves method, http version and url.
     */
    void _parse_request_line();
    /**
     * @brief parse request headers.
     */
    void _parse_headers();

    /* Change that to ranges ? */
    std::string path_;
    std::string url_;
    std::string method_;
    std::string raw_;
    std::string request_line_;
    HeadersMap _headers;
    bool _headers_parsed;
    HttpVersion _http_version;
    Client &_client;
    int status_code_;
    Context& context_;
    Response _response;
};
}

#endif
