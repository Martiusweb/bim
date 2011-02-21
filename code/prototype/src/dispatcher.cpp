/**
 * This file is part of Bim, the web server which goes fast.
 * © Martin Richard & Paul Adenot
 *
 * Bim is free software : you can redistribute it and/or modify it under the
 * terms of the license.
 */


#include "dispatcher.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <sys/epoll.h>

namespace bim
{
    dispatcher::dispatcher(int max_events):
        _epoll(0), _max_events(max_events)
    {
    }

    dispatcher::~dispatcher()
    {
        if(_epoll != 0) {
            close();
        }
    }

    bool dispatcher::open()
    {
        if((_epoll = epoll_create(_max_events)) == -1) {
            _epoll = 0;
            return false;
        }
        return true;
    }

    void dispatcher::close()
    {
        ::close(_epoll);
    }

    void dispatch()
    {
    }
}

