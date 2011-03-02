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
 * File author : Paul ADENOT <paul@paul.cx>
 *
 **/


#include "client.h"

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

