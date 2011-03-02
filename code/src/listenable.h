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


#ifndef BIM_LISTENABLE_H
#define BIM_LISTENABLE_H

namespace bim {

class EventDispatcher;

class Listenable
{
    public:
        Listenable();
        virtual ~Listenable();
        inline bool isReady() const {
            return _descriptor > 0;
        }
        inline int getDescriptor() const {
            return _descriptor;
        }
        
        /**
         * @brief Registers callbacks to the event dispatcher.
         * @param ed The event dispatcher instance
         * The base version does not register callbacks, you have to do it in
         * inherited classes with listenIn()/listenOut() and cie.
         * Will register the event dispatcher in an internal attribute and
         * returns true.
         */
        virtual bool registerEventDispatcher(EventDispatcher& ed);
        void unregisterEventDispatcher();
        virtual void onIn() {};
        virtual void onOut() {};
        virtual void onErr() {};

    protected:
        int _descriptor;
        EventDispatcher* _event_dispatcher;
};
} // /bim

#include "event_dispatcher.h"

#endif

