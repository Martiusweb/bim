#ifndef _METHOD_VALIDATOR_
#define _METHOD_VALIDATOR_

#include "handler.h"

class MethodValidator : public Handler
{
  public: 
    MethodValidator();
    void handle(Request& request);
};


