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


#include "client.h"
#include "thread_pool.h"
#include "read_job.h"
#include "macros.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <iostream>
#include <string.h>


namespace bim {
Client::Client(ThreadPool& pool, Context& context)
  : Listenable(), _handled_requests(0), _server(0), thread_pool_(pool),
  context_(context), _queued_requests() {
    bzero((char *) &_address, sizeof(_address));
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
    _server = &server;
    socklen_t addrln = sizeof(_address);
    int flags = 0;

    if(pthread_mutex_init(&_queue_mutex, 0) != 0) {
      return false;
    }

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
    DBG_LOG("Client (" << this << ") connected");
    return true;
}

void Client::close() {
    DBG_LOG(_handled_requests << " request(s) handled");
    ::close(_descriptor);
    _descriptor = 0;

    Request* r;
    pthread_mutex_lock(&_queue_mutex);
    while(!_queued_requests.empty()) {
      r = _queued_requests.front();
      _queued_requests.pop();
      delete r;
    }
    pthread_mutex_unlock(&_queue_mutex);

    pthread_mutex_destroy(&_queue_mutex);
}

bool Client::registerEventDispatcher(EventDispatcher& ed) {
    if(ed.listenInOut(this)) {
        return Listenable::registerEventDispatcher(ed);
    }

    return false;
}

void Client::requestHandled(Request* request) {
  pthread_mutex_lock(&_queue_mutex);
  ++_handled_requests;
  _queued_requests.push(request);
  pthread_mutex_unlock(&_queue_mutex);
}

void Client::requestsRead() {
}

void Client::requestParsed() {
}

void Client::requestProcessed() {
  bool keep_alive;

  pthread_mutex_lock(&_queue_mutex);
  Request* processed = _queued_requests.front();
  _queued_requests.pop();

  keep_alive = processed->keepAlive() || !_queued_requests.empty();
  pthread_mutex_unlock(&_queue_mutex);
  // Good bye request ! Paul liked you !

  delete processed;

  if(!keep_alive) {
    DBG_LOG("Client (" << this << ") disconnected by server, ");
    close();
    _server->clientDisconnected(this);
  }
}

void Client::onIn() {
  thread_pool_.postJob(new ReadJob(thread_pool_, *this, context_));
}

void Client::onOut() {
}

void Client::onErr() {
  DBG_LOG("client closed the connection (" << this << ")");
  close();
  // We should not consider client dead since we don't know if the workers are
  // terminated
  //_server->clientDisconnected(this);
}

} // /bim

