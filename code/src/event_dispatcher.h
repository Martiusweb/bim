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


#ifndef BIM_EVENTDISPATCHER_H
#define BIM_EVENTDISPATCHER_H

namespace bim {
class Listenable;

/**
 * The event dispatcher will poll, collect and dispatch I/O events detected on
 * Listenable objects.
 */
class EventDispatcher
{
    public:
        enum ListenOperation {CREATE, UPDATE};

        /**
         * @brief Constructs the dispatcher.
         * @param nb_max_events Maximum of events collectables
         * @param events_per_loop Number of events managed in one loop
         *
         * The object is created in an inactive stable mode.
         */
        EventDispatcher(int nb_max_events = 1, int events_per_loop = 1);

        /**
         * @brief Destructs the dispatcher
         */
        ~EventDispatcher();

        /**
         * @brief Initialise the object.
         * It will create the mandatory objects. It must be called once before
         * any other operation.
         */
        bool init();

        /**
         * @brief Closes internal objects.
         * It will be called if needed during the object destruction.
         */
        void close();

        /**
         * @brief Start listening input events on a Listenable.
         * @param listenable Listenable object
         * @param update CREATE or UPDATE listened events
         *
         * Do not try to update the state of a Listenable unknown to the
         * disptacher.
         */
        bool listenIn(Listenable* const listenable,
                ListenOperation update = CREATE);

        /**
         * @brief Start listening output events on a Listenable.
         * @param listenable Listenable object
         * @param update CREATE or UPDATE listened events
         *
         * Do not try to update the state of a Listenable unknown to the
         * disptacher.
         */
        bool listenOut(Listenable* const listenable,
                ListenOperation update = CREATE);
        
        /**
         * @brief Start listening input and output events on a Listenable.
         * @param listenable Listenable object
         * @param update CREATE or UPDATE listened events
         *
         * Do not try to update the state of a Listenable unknown to the
         * disptacher.
         */
        bool listenInOut(Listenable* const listenable,
                ListenOperation update = CREATE);

        /**
         * @brief stop listening events on a Listenable.
         *
         * Do not try to stop listening a Listenable unknown to the disptacher.
         */
        void stopListening(Listenable &listenable);

        /**
         * @brief Start dispatching events.
         *
         * This method is blocking and will never stops.
         */
        void dispatch();

    protected:
        /**
         * @brief Factored method listening control.
         * @param listenable Listenable object
         * @param update CREATE if the object is not known by the dispatcher
         * @param mode E-poll events to listen (bitmask)
         */
        bool _listen(Listenable* const listenable, ListenOperation update,
                int mode);

        /**
         * @brief E-poll descriptor.
         */
        int _epoll;

        /**
         * @brief Maximum of events kept by epoll simultaneously.
         */
        int _nb_max_events;

        /**
         * @brief Maximum of events fetched in one loop.
         */
        int _events_per_loop;
};
} // /bim


#endif

