/**
 * This file is part of Bim, the web server which goes fast.
 * © Martin Richard & Paul Adenot
 *
 * Bim is free software : you can redistribute it and/or modify it under the
 * terms of the license.
 */


#include "server.hpp"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace bim
{
    server::server() : _socket(0)
    {
    }

    bool server::open(unsigned int port, unsigned int max_clients)
    {
        struct sockaddr_in addr_server = {0};

        _socket = socket(AF_INET, SOCK_STREAM, 0);
        if(_socket == -1) {
            _socket = 0;
            return false;
        }
        else {
            addr_server.sin_family = AF_INET;
            addr_server.sin_port = htons(port);
            addr_server.sin_addr.s_addr = htonl(INADDR_ANY);

            if(bind(_socket, (struct sockaddr*) &addr_server,
                        sizeof(addr_server)) != 0) {
                close();
                return false;
            }

            if(listen(_socket, max_clients) < 0) {
                close();
                return false;
            }
            return true;
        }
    }

    void server::close()
    {
        ::close(_socket);
        _socket = 0;
    }

    server::~server()
    {
        if(_socket != 0)
            close();
    }
}
