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


#include "server.h"
#include "client.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

namespace bim {
Server::Server(int port, int max_clients, ThreadPool& pool, Context& context)
  :Listenable()
  ,_port(port)
  ,_max_clients(max_clients)
  ,thread_pool_(pool)
  ,context_(context)
{ }

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
    Client* client = new Client(thread_pool_, context_);
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

