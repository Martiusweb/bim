/// Author : Paul ADENOT <paul@paul.cx>
/// License : WTFPL <http://sam.zoy.org/wtfpl/>

#include "request.h"
#include "context.h"
#include "assert.h"
#include <iostream>

namespace bim 
{

using namespace std;

Request::Request(const int fd, Context& context)
:fd_(fd)
,context_(context)
{}

std::string& Request::get_method()
{
  if(method_.empty())
  {
    size_t pos = raw_.find_first_of(' ');

    if(pos != std::string::npos)
    {
      method_ = raw_.substr(0,pos);
    }
  }
  return method_;
}

std::string& Request::get_url()
{
  if(url_.empty())
  {
    size_t begin_url = raw_.find_first_of(' ');
    size_t end_url = raw_.find_first_of(' ', begin_url+1);

    if(begin_url != string::npos && end_url != string::npos)
    {
      url_ = raw_.substr(begin_url+1, end_url - begin_url - 1); 
    }
  }
  return url_;
}

std::string& Request::get_path()
{
  if(path_.empty())
  {
    path_ = context_.get_document_root()+get_url();
  }
  return path_;
}


int Request::get_fd()
{
  return fd_;
}


void Request::append_data(const char* data)
{
  raw_ += data;
}

}

