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


#ifndef BIM_CLIENT_H
#define BIM_CLIENT_H

#include "listenable.h"
#include "server.h"

#include <arpa/inet.h>

namespace bim {
class ThreadPool;
/**
 * A client object represent a connection of a client through socket.
 */
class Client: public Listenable
{
    public:
        Client(ThreadPool& pool, Context& context);
        virtual ~Client();
        /**
         * @brief accept a client
         * Accepts connection and set the client socket non blocking. These
         * operations are mandatory. Returns false if any operation fails.
         */
        bool initialize(Server& server);

        /**
         * @brief close a client connection
         */
        void close();

        virtual bool registerEventDispatcher(EventDispatcher& ed);

        /**
         * @brief Data is available.
         */
        virtual void onIn();
        virtual void onOut();
        virtual void onErr();

    protected:
        /**
         * @brief Low-level structure address.
         */
        struct sockaddr_in6 _address;

        /**
         * @brief Number of requests the current socket transmited.
         *
         * This value is used in the context of persistent connections, and
         * is used to limit the number of requests a single connection will
         * handle.
         */
        int _handled_requests;

        /**
         * @brief pointer to the server that accepted the connection.
         */
        Server* _server;

        /**
         * @brief Thread pool that will manage the jobs processing.
         */
        ThreadPool& thread_pool_;

        /**
         * @brief context object.
         */
        Context& context_;
};
} // /bim

#endif

