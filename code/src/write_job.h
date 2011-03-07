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

#ifndef WRITE_JOB_H
#define WRITE_JOB_H

#include <string>

#include "http_status_code.h"
#include "job.h"

namespace bim
{

class ThreadPool;
class Context;

class WriteJob : public Job
{
  public:
    /**
     * This specifies what to do with the data (i.e. which constructor has been
     * called).
     */
    enum ContentType
    {
      Data,
      Path
    };
    /**
     * @brief A job to write the request back to the client
     *
     * @param pool The thread pool, to post job back (actually, for 
     * consistency).
     * @param context The context, to get contextual informations.
     * @param data The data (file or path).
     * @para type The type of the content of data : file or path.
     * @param code An optionnal error code to include in the header. 200 (OK)
     * is assumed if none is passed.
     */
    WriteJob(ThreadPool& pool,
             Context& context,
             const std::string& data,
             const ContentType type = Path,
             const HttpStatusCode code = OK_200);

    Action act();

  private:
    /**
     * This can either be the data to send back or the path of the file.
     */
    const std::string& data_;
    ContentType buffer_content_;
    /**
     * The error code to put in the header
     */
    const HttpStatusCode code_;

};

}

#endif
