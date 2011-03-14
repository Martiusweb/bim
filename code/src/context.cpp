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

#include <iostream>

#include "context.h"

namespace bim
{
  static const std::string DEFAULT_CONTENT_TYPE = "text/html";

  Context::Context()
  {
    const std::string prefix = "errors/";
    error_path_[OK_200] = prefix + "ok.html";
    //error_path_[CREATED_201] = prefix + 
    //error_path_[ACCEPTED_202] = prefix + 
    //error_path_[NO_CONTENT_204] = prefix + 
    //error_path_[MULTIPLE_CHOICE_300] = prefix + 
    //error_path_[MOVED_PERMANENTLY_301] = prefix + 
    //error_path_[MOVED_TEMPORARILY_302] = prefix + 
    //error_path_[NOT_MODIFIED_304] = prefix + 
    error_path_[BAD_REQUEST_400] = prefix + "400.html";
    error_path_[UNAUTHORIZED_401] = prefix + "401.html";
    error_path_[FORBIDDEN_403] = prefix + "403.html";
    error_path_[NOT_FOUND_404] = prefix + "404.html";
    error_path_[INTERNAL_SERVER_ERROR_500] = prefix + "500.html";
    error_path_[NOT_IMPLEMENTED_501] = prefix + "501.html";
    error_path_[BAD_GATEWAY_502] = prefix + "502.html";
    error_path_[SERVICE_UNAVAILABLE_503] = prefix + "503.html";
  }

  std::string& Context::getDocumentRoot()
  {
      return document_root_;
  }

  void Context::setDocumentRoot(const std::string& document_root)
  {
    document_root_=document_root;
    if(document_root_[document_root_.size() - 1] == '/')
    {
      document_root_.erase(document_root_.size() - 1);
    }
    std::cout << "document_root:" << document_root_ << ":" << std::endl;
  }

  std::string& Context::getErrorDocumentPath(const HttpStatusCode code)
  {
    return error_path_[code];
  }

  void Context::setErrorDocumentPath(const HttpStatusCode code,
      const std::string& path)
  {
    error_path_[code] = path;
  }

  std::string Context::parseContentType(const std::string &file_path) const {
    size_t extpos = file_path.find_last_of('.');
    if(extpos == file_path.npos) {
      return DEFAULT_CONTENT_TYPE;
    }
    std::string ext = file_path.substr(extpos+1);

    if(ext == "html" || ext == "htm")
      return "text/html";
    if(ext == "css")
      return "text/css";
    if(ext == "javascript")
      return "text/javascript";
    if(ext == "json")
      return "application/json";
    if(ext == "jpg")
      return "image/jpeg";
    if(ext == "png")
      return "image/png";
    if(ext == "gif")
      return "image/gif";

    return DEFAULT_CONTENT_TYPE;
  }
}

