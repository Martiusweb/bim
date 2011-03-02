#include "Server.hpp"
#include "Client.hpp"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

namespace bim {
Server::Server(int port, int max_clients):
    Listenable(), _port(port), _max_clients(max_clients) {
}

Server::~Server() {
    if(_event_dispatcher != 0) {
        unregisterEventDispatcher();
    }
    if(_descriptor != 0) {
        close();
    }
}

bool Server::init() {
    struct sockaddr_in6 addr;
    int flags = 1;

    _descriptor = socket(AF_INET6, SOCK_STREAM, 0);

    if(_descriptor < 0) {
        _descriptor = 0;
        return false;
    }

    // Linux only : by default, an IPv6 socket is backward compatible.
    // This is not the common case !
    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(_port);
    addr.sin6_addr = in6addr_any;

    if(bind(_descriptor, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
        close();
        return false;
    }

    if(listen(_descriptor, _max_clients) < 0) {
        close();
        return false;
    }

    setsockopt(_descriptor, SOL_SOCKET, SO_REUSEADDR, &flags, sizeof(int));

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

bool Server::registerEventDispatcher(EventDispatcher& ed) {
    if(ed.listenIn(this, false)) {
        return Listenable::registerEventDispatcher(ed);
    }

    return false;
}

void Server::onIn()
{
    Client* client = new Client();
    if(!(client->initialize(*this) &&
                client->registerEventDispatcher(*_event_dispatcher))) {
        delete client;
    }
}

void Server::onErr() {
}

void Server::close() {
    ::close(_descriptor);
    _descriptor = 0;
}

} // /bim

