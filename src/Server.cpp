#include "Server.hpp"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

namespace bim {
Server::Server(int port, int max_clients):
    Listenable(), _port(port), _max_clients(max_clients) {
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

bool Server::registerEventDispatcher(EventDispatcher& ed)
{
    return ed.listenIn(this, false);
}

void Server::onIn()
{
    // perform accept
}

void Server::close() {
    ::close(_descriptor);
    _descriptor = 0;
}

Server::~Server() {
    if(_descriptor != 0) {
        close();
    }
}

} // /bim

