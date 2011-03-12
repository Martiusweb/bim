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


#ifndef BIM_SERVER_H
#define BIM_SERVER_H

#include "listenable.h"
#include "thread_pool.h"
#include "context.h"

namespace bim {
class Client;
/**
 * @brief Manages a listening socket
 * IO operations are non blocking. You must use an event-based model to try to
 * find out when the socket is ready for IO operations.
 *
 * Socket is implicitely closed when the object is destructed if it was still
 * opened.
 */
class Server: public Listenable
{
    public:
        Server();
        Server(int port,
               int max_clients,
               ThreadPool& pool,
               Context& context);
        virtual ~Server();

        /**
         * @brief initialize a server socket
         * Open a new stream socket, bind and listen to incomming
         * connections.
         *
         * Many operations are performed, they must all succeed. The "reuse
         * address" flag will be set, this is the only operation that can
         * fail without check.
         * 
         * If true is returned, the whole initialization had been performed,
         * else, the socket is closed and false is returned.
         */
        bool init();

        /**
         * @brief closes the server socket
         * Does not tests if the socket is opened.
         */
        void close();

        void clientDisconnected(Client* client) const;

        virtual bool registerEventDispatcher(EventDispatcher& ed);
        virtual void onIn();
        virtual void onErr();
        virtual void onOut();

    protected:
        /**
         * @brief Listening port.
         */
        int _port;
        /**
         * @brief Maximum of clients handled simultaneously by the server
         */
        int _max_clients;
        /**
         * @brief Thread pool processing jobs
         */
        ThreadPool& thread_pool_;
        /**
         * @brief Context data
         */
        Context& context_;
};
} // /bim

#include "client.h"
#endif

