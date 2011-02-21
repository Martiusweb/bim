/**
 * This file is part of Bim, the web server which goes fast.
 * © Martin Richard & Paul Adenot
 *
 * Bim is free software : you can redistribute it and/or modify it under the
 * terms of the license.
 */

namespace bim {
    class dispatcher
    {
        public:
        dispatcher(int max_events);
        ~dispatcher();
        bool open();
        void close();
        void dispatch();

        protected:
        int _epoll;
        int _max_events;
    };
}

