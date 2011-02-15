#include "handler.h"

Handler::Handler()
: next_(0)
{

}

Handler::~Handler()
{
  delete next_;
}

void Handler::handle(Request& request)
{
  next_->handle(request, fd);
}

void Handler::add(Handler* next_handler)
{
  if(next_)
  {
    next_->add(next_handler);
  }
  else
    next_ = next_handler;
}

