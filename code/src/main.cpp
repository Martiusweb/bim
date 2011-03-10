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


#include <iostream>
#include "server.h"
#include "event_dispatcher.h"
#include "macros.h"

using namespace bim;
using namespace std;

/**
 * This is the Bim entry point for tests and standalone server.
 * It shows how the Bim API can be used to create an http server but must not
 * be used for production : use a daemon instead !
 *
 * As you will see, there is no way to stop the program : use Ctrl+C.
 */
int main(int /* argc */, char** /* argv */)
{
    // The context provide informations the server will use as configuration
    // keys
    Context context;
    context.setDocumentRoot("document_root");

    // The thread pool is the object that manage how the server jobs are
    // processed
#ifdef BIM_THREADPOOL_THREADS_PER_CORE
    ThreadPool pool(BIM_THREADPOOL_THREADS_PER_CORE);
#else
    ThreadPool pool;
#endif

    // The server object, do we need to say more ?
    Server server(7000, 10000, pool, context);

    // The event dispatcher notifies server and clients when they can act, then
    // they will use the thread pool to work.
    EventDispatcher dispatcher(1024, 16);

    DBG_LOG("Welcome, ready to initialize");

    // Order does not matters here
    TEST_FAILURE(!(pool.init() && dispatcher.init() && server.init()));

    // server and dispatcher must be initialized
    server.registerEventDispatcher(dispatcher);

    // Contains the event-loop, get and dispatch i/o events
    dispatcher.dispatch();

    return 0;
}

