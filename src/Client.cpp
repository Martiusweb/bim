#include "Client.hpp"

#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

namespace bim {
Client::Client(): Listenable() {
}

Client::~Client() {
    if(_event_dispatcher != 0) {
        unregisterEventDispatcher();
    }
    if(_descriptor != 0) {
        close();
    }
}

bool Client::initialize(Server &server) {
    socklen_t addrln = sizeof(_address), flags = 0;
    if((_descriptor = accept(server.getDescriptor(), (sockaddr*) &_address,
                    &addrln)) == -1) {
        _descriptor = 0;
        return false;
    }
    
    if((flags = fcntl(_descriptor, F_GETFL, 0)) == -1) {
        close();
        return false;
    }

    if(fcntl(_descriptor, F_SETFL, flags|O_NONBLOCK) == -1) {
        close();
        return false;
    }
    return true;
}

void Client::close() {
    ::close(_descriptor);
    _descriptor = 0;
}

bool Client::registerEventDispatcher(EventDispatcher& ed) {
    if(ed.listenInOut(this, true)) {
        return Listenable::registerEventDispatcher(ed);
    }

    return false;
}

void Client::onIn() {
}

void Client::onOut() {
}

void Client::onErr() {
}

} // /bim

