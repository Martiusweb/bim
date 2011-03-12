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

#ifndef BIM_RESPONSE_H
#define BIM_RESPONSE_H

#include <string>

#include "http_status_code.h"

namespace bim
{

class Response
{
  public:
    /**
     * @brief Constructor of a response object.
     */
    Response(HttpStatusCode code = OK_200);

    /**
     * @brief Returns a string of the response header
     */
    std::string getHeader();

    /**
     * @brief allows to update the request status code.
     *
     * It will be useless if header had been sent.
     */
    void setStatusCode(HttpStatusCode code);

    /**
     * @brief Returns the status code of the response.
     */
    inline HttpStatusCode getStatusCode() {
      return _status_code;
    }

    /**
     * @brief Notify that response header had been sent to client.
     */
    void setHeaderSent();

    /**
     * @brief Returns true if the headers had been sent.
     */
    inline bool areHeadersSent() {
      return _headers_sent;
    }

  protected:
    HttpStatusCode _status_code;
    bool _headers_sent;
};

}

#endif

