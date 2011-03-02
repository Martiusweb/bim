#include "EventDispatcher.hpp"

#include <sys/epoll.h>
#include <unistd.h>

namespace bim {

EventDispatcher::EventDispatcher(int nb_max_events, int events_per_loop):
    _epoll(0), _nb_max_events(nb_max_events), _events_per_loop(events_per_loop)
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
    ::close(_epoll);
}

EventDispatcher::~EventDispatcher()
{
    if(_epoll != 0) {
        close();
    }
}

bool EventDispatcher::_listen(Listenable* const listenable, bool update, int mode)
{
    epoll_event event;
    event.events = mode | EPOLLET;
    event.data.ptr = (void*) listenable;

    if(epoll_ctl(_epoll, (update ? EPOLL_CTL_MOD : EPOLL_CTL_ADD),
                listenable->getDescriptor(), &event) == -1) {
        return false;
    }
    return true;
}

bool EventDispatcher::listenIn(Listenable* const listenable, bool update)
{
    return _listen(listenable, update, EPOLLIN);
}

bool EventDispatcher::listenOut(Listenable* const listenable, bool update)
{
    return _listen(listenable, update, EPOLLOUT);
}

bool EventDispatcher::listenInOut(Listenable* const listenable, bool update)
{
    return _listen(listenable, update, EPOLLIN | EPOLLOUT);
}

void EventDispatcher::stopListening(Listenable& listenable) {
    /* static */ epoll_event event;
    epoll_ctl(_epoll, EPOLL_CTL_DEL, listenable.getDescriptor(), &event);
}

void EventDispatcher::dispatch()
{
    int nb_events_fetched, i;
    epoll_event *events = new epoll_event[_events_per_loop];
    Listenable* listenable;

    for(;;) {
        // We wait infinitely for an event
        nb_events_fetched = epoll_wait(_epoll, events, _events_per_loop, -1);
        for(i = 0; i < nb_events_fetched; ++i) {
            listenable = (Listenable*) events[i].data.ptr;

            // TODO Here we connect the job list
            if(events[i].events & (EPOLLHUP|EPOLLERR)) {
                listenable->onErr();
            }
            if(events[i].events & EPOLLIN) {
                listenable->onIn();
            }
            if(events[i].events & EPOLLERR) {
                listenable->onOut();
            }
        }
        
    }
    delete[] events;
}

} // /bim

