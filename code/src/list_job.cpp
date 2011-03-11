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

#include <algorithm>
#include <assert.h>
#include <dirent.h>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <sys/types.h>

#include "list_job.h"
#include "macros.h"
#include "request.h"
#include "write_job.h"

namespace bim
{

  static const char* const HTML_BEGIN = "<html><head><title>Index of ";
  static const char* const HTML_BEGIN2 = "</title></head><body><pre>";
  static const char* const HTML_END = "bim webserver -- 0.1</pre></body></html>";
  static const char* const BR = "<br>";
  static const char* const HR = "<hr>";
  static const char* const A_BEGIN = "<a href=";
  static const char* const A_END1= ">";
  static const char* const A_END2= "</a>";
  static const char* const DIR_LABEL = "[DIR]\t";
  static const char* const FILE_LABEL = "[REG]\t";
  static const char* const TITLE_BEGIN = "<h1> Index of ";
  static const char* const TITLE_END = "</h1>";

  ListJob::ListJob(ThreadPool& pool, Context& context, Request& request)
    :Job(pool, context)
     ,request_(request)
  { }

  Action ListJob::act()
  {
    DIR* dir  = opendir(request_.getPath().c_str());
    struct dirent* entry = 0;
    struct dirent* entryp = 0;

    // prefered way to allocate for readdir_r usage (see man 2 readdir).
    size_t len = offsetof(struct dirent, d_name) +
      pathconf(request_.getPath().c_str(), _PC_NAME_MAX) + 1;

    entry = reinterpret_cast<dirent*>(new char[len]);

    // operator+ would create temporary std::string.
    // we try to avoid this here.
    html_page_ = HTML_BEGIN;
    html_page_ += request_.getUrl();
    html_page_ += HTML_BEGIN2;
    html_page_ += TITLE_BEGIN;
    html_page_ += request_.getUrl();
    html_page_ += TITLE_END;
    html_page_ += HR;

    std::deque<std::string> entries;
    std::string point_point_line;
    while(readdir_r(dir, entry, &entryp) == 0)
    {
      if(entryp)
      {
        // dont list . (current dir), nor .. if we are at /
        if( (! strcmp(entryp->d_name,"..") && request_.getUrl() == "/")
            || ! strcmp(entry->d_name, ".")
            || entryp->d_type == DT_LNK )
        {
          continue;
        }
        std::string line;
        bool is_point_point = false;
        if(entryp->d_type == DT_REG)
        {
          line += FILE_LABEL;
        }
        if(entryp->d_type == DT_DIR)
        {
          line += DIR_LABEL;
        }
        line += A_BEGIN;
        if( ! strcmp(entryp->d_name,".."))
        {
          // get parent directory
         line +=  get_parent(request_.getUrl());
         is_point_point = true;
        }
        else
        {
          if(request_.getUrl() != "/")
          {
            line += request_.getUrl();
            if(request_.getUrl()[request_.getUrl().size()-1] != '/')
            {
              line += "/";
            }
          }
          line += entryp->d_name;
        }
        line += A_END1;
        line += entryp->d_name;
        line += A_END2;
        line += "\t";
        line += BR;

        // the .. dir is stored apart, it shall be at the begin of the list
        if(is_point_point)
        {
          point_point_line = line;
        }
        else
        {
          entries.push_back(line);
        }
      }
      else
      {
        break;
      }
    }

    std::sort(entries.begin(), entries.end());
    entries.push_front(point_point_line);
    std::deque<std::string>::iterator e = entries.end();

    for(std::deque<std::string>::iterator i = entries.begin(); i != e; i++)
    {
      html_page_ += *i;
    }

    html_page_ += HR;
    html_page_ += HTML_END;

    delete [] reinterpret_cast<char*>(entry);
    TEST_FAILURE(closedir(dir));

    pool_.postJob(new WriteJob(pool_, context_,
                               request_.getFd(),
                               html_page_,
                               WriteJob::Data));

    return Delete;
  }

  std::string ListJob::get_parent(std::string& path)
  {
    char lastchar = path[path.size()-1];

    if(lastchar == '/')
    {
      size_t i = path.size() - 1; 
      while(path[--i] != '/');
      return path.substr(0, i+1);
    }

    return path.substr(0, path.find_last_of("/") + 1);
  }
}

