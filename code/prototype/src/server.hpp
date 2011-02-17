/**
 * This file is part of Bim, the web server which goes fast.
 * © Martin Richard & Paul Adenot
 *
 * Bim is free software : you can redistribute it and/or modify it under the
 * terms of the license.
 */

#include <sys/types.h>

namespace bim {
    class server
    {
        public:
        server();
        ~server();
        bool open(unsigned int port, unsigned int max_clients);
        void close();

        protected:
        int _socket;
    };
}

