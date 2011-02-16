/**
 * This file is part of Bim, the web server which goes fast.
 * © Martin Richard & Paul Adenot
 *
 * Bim is free software : you can redistribute it and/or modify it under the
 * terms of the license.
 */

#include <sys/types.h>

namespace bim {
    class dispatcher
    {
        public:
        dispatcher(max_events);
        ~dispatcher();
        void dispatch();

        protected:
        int _epoll;
    };
}

