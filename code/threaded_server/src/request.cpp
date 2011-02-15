
#include "request.h"""

Request::Request(const std::string& http_request, const int fd, const Context& context)
:raw_(http_request)
,fd_(fd)
{}

std::string& Request::get_method()
{
  if(method_.empty())
  {
    size_t pos = raw_.find_first_of(' ');

    if(pos != string::npos)
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
    size_t end_url = raw_.find_first_of(' ', begin_url);

    if(begin_url != string::npos && end_url != string::npos)
    {
      url_ = raw_.substr(begin_url + 1, end_url - begin_url); 

      assert(url_.find_first_of(' ') == string::npos);
    }
  }
  return url_;
}

std::string& Request::get_path()
{
}


int Request::get_fd()
{
  return fd;
}
