#ifndef BIM_EVENTDISPATCHER_H
#define BIM_EVENTDISPATCHER_H

namespace bim {
class Listenable;

class EventDispatcher
{
    public:
        EventDispatcher(int nb_max_events = 1, int events_per_loop = 1);
        ~EventDispatcher();
        bool init();
        void close();
        bool listenIn(Listenable* const listenable, bool update = false);
        bool listenOut(Listenable* const listenable, bool update = false);
        bool listenInOut(Listenable* const listenable, bool update = false);
        void stopListening(Listenable &listenable);
        void dispatch();

    protected:
        bool _listen(Listenable* const listenable, bool update, int mode);
        int _epoll;
        int _nb_max_events;
        int _events_per_loop;
};
} // /bim

#include "Listenable.hpp"

#endif

