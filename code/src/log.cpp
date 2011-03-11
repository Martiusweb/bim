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

#include <time.h>
#include <cstdlib>

#include "log.h"

namespace bim
{
  Log *Log::singleton_ = 0;

  const char* Log::error_log_file_ = "error_log";
  const char* Log::access_log_file_= "access_log";
  const char* Log::trace_log_file_ = "trace_log";

  void access_log(const std::string& message)
  {
    Log::get_log()->write(message, Log::Access);
  }

  void error_log(const std::string& message)
  {
    Log::get_log()->write(message, Log::Error);
  }

  void trace_log(const std::string& message)
  {
    Log::get_log()->write(message, Log::Trace);
  }

  void all_log(const std::string& message)
  {
    Log::get_log()->write(message, Log::All);
  }

  Log::Log()
  {
    try
    {
      error_log_.open(error_log_file_, std::ios_base::out|std::ios_base::app);
    }
    catch(...)
    {
      std::cout << "error : can't open error log" << std::endl;
      exit(EXIT_FAILURE);
    }

    try
    {
      access_log_.open(access_log_file_, std::ios_base::out|std::ios_base::app);
    }
    catch(...)
    {
      std::cerr << "error : can't open access log" << std::endl;
      exit(EXIT_FAILURE);
    }

    try
    {
      access_log_.open(trace_log_file_, std::ios_base::out|std::ios_base::app);
    }
    catch(...)
    {
      std::cerr << "error : can't open trace log" << std::endl;
      exit(EXIT_FAILURE);
    }

    pthread_mutex_init(&write_access_, NULL);
    pthread_mutex_init(&write_error_, NULL);
    pthread_mutex_init(&write_trace_, NULL);
    write("-- Log opening", All);
  }

  Log::~Log()
  {
  }

  Log * Log::get_log()
  {
    if( ! singleton_ )
      singleton_ = new Log();
    return singleton_;
  }

  void Log::write(const std::string& message, LogFile file)
  {
    time_t t = time(0);
    std::string time(ctime(&t));
    time[time.size() - 1] = 0;

    if(file & Access)
    {
      pthread_mutex_lock(&write_access_);
      access_log_ << "[" << time << "] # " << pthread_self()  << " # " << message << std::endl;
      pthread_mutex_unlock(&write_access_);
    }

    if(file & Error)
    {
      pthread_mutex_lock(&write_error_);
      error_log_ << "[" << time << "] # " << pthread_self()  << " # " << message << std::endl;
      pthread_mutex_unlock(&write_error_);
    }

    if(file & Trace)
    {
      pthread_mutex_lock(&write_trace_);
      trace_log_ << "[" << time << "] # " << pthread_self()  << " # " << message << std::endl;
      pthread_mutex_unlock(&write_trace_);
    }
  }

  void Log::close()
  {
    write("-- LOG CLOSING", All);
    pthread_mutex_lock(&write_access_);
    access_log_.close();
    pthread_mutex_unlock(&write_access_);

    pthread_mutex_lock(&write_access_);
    error_log_.close();
    pthread_mutex_unlock(&write_access_);

    pthread_mutex_lock(&write_trace_);
    trace_log_.close();
    pthread_mutex_lock(&write_trace_);

    pthread_mutex_destroy(&write_access_);
    pthread_mutex_destroy(&write_error_);
    pthread_mutex_destroy(&write_trace_);
    delete singleton_;
    singleton_ = 0;
  }
}
