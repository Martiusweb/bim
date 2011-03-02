#include "Listenable.hpp"

namespace bim {
Listenable::Listenable():
    _descriptor(0), _event_dispatcher(0) {
}

Listenable::~Listenable() {
    if(_event_dispatcher != 0) {
        unregisterEventDispatcher();
    }
}

bool Listenable::registerEventDispatcher(EventDispatcher& ed) {
    _event_dispatcher = &ed;
    return true;
}

void Listenable::unregisterEventDispatcher() {
    _event_dispatcher->stopListening(*this);
    _event_dispatcher = 0;
}

} // /bim

