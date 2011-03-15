/**
 * Copyright (c) 2011, Paul ADENOT & Martin Richard
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * - Neither the name of the bim nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
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


#include "event_dispatcher.h"
#include "listenable.h"
#include "macros.h"

#include <sys/epoll.h>
#include <unistd.h>
#include <cstdio>
#include <cassert>

namespace bim {

EventDispatcher::EventDispatcher(int nb_max_events, int events_per_loop):
    _epoll(0), _nb_max_events(nb_max_events),
    _events_per_loop(events_per_loop),
    closing_requested_(false)
{
}

bool EventDispatcher::init()
{
    if((_epoll = epoll_create(_nb_max_events)) == -1) {
        _epoll = 0;
        return false;
    }
    return true;
}

void EventDispatcher::close()
{
    closing_requested_ = true;
    ::close(_epoll);
    _epoll = 0;
}

EventDispatcher::~EventDispatcher()
{
    if(_epoll != 0) {
        close();
    }
}

bool EventDispatcher::_listen(Listenable* const listenable,
        ListenOperation update, int mode)
{
    epoll_event event = {0,{0}};
    event.events = mode | EPOLLET | EPOLLRDHUP | EPOLLHUP;
    event.data.ptr = reinterpret_cast<void*>(listenable);

    if(epoll_ctl(_epoll, (update ? EPOLL_CTL_MOD : EPOLL_CTL_ADD),
                listenable->getDescriptor(), &event) == -1) {
        return false;
    }
    return true;
}

bool EventDispatcher::listenIn(Listenable* const listenable,
        ListenOperation update)
{
    return _listen(listenable, update, EPOLLIN);
}

bool EventDispatcher::listenOut(Listenable* const listenable,
        ListenOperation update)
{
    return _listen(listenable, update, EPOLLOUT);
}

bool EventDispatcher::listenInOut(Listenable* const listenable,
        ListenOperation update)
{
    return _listen(listenable, update, EPOLLIN | EPOLLOUT);
}

void EventDispatcher::stopListening(Listenable& listenable) {
    epoll_event event = {0,{0}};
    epoll_ctl(_epoll, EPOLL_CTL_DEL, listenable.getDescriptor(), &event);
}

void EventDispatcher::dispatch()
{
    int nb_events_fetched, i;
    static int total_events_fetched_out = 0;
    static int total_events_fetched_in = 0;
    epoll_event *events = new epoll_event[_events_per_loop];
    Listenable* listenable;

    while( ! closing_requested_ ) {
        // We wait infinitely for an event
        nb_events_fetched = epoll_wait(_epoll, events, _events_per_loop, 5);
        assert(nb_events_fetched <= _events_per_loop);
        TEST_FAILURE_MINUS_ONE(nb_events_fetched);
        for(i = 0; i < nb_events_fetched; ++i) {
            listenable = (Listenable*) events[i].data.ptr;
 
            if(events[i].events & (EPOLLHUP|EPOLLERR|EPOLLRDHUP)) {
              error_log("got error");
                listenable->onErr();
            }
            else {
              if(events[i].events & EPOLLIN) {
                  listenable->onIn();
                  std::stringstream strs;
                  strs << "nb fetched in: " << total_events_fetched_in++;
                  trace_log(strs.str());
              }
              // Cannot be EPOLLERR there !
              if(events[i].events & EPOLLOUT) {
                  listenable->onOut();
                  std::stringstream strs;
                  strs << "nb fetched out: " << total_events_fetched_out++;
                  trace_log(strs.str());
              }
            }
        }
    }

    delete [] events;
}

} // /bim

