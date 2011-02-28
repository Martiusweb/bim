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
    struct epoll_event event;
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

void EventDispatcher::dispatch()
{
    // event loop
    // listenable.onIn() etc
}

} // /bim

