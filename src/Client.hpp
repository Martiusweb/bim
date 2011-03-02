#ifndef BIM_CLIENT_H
#define BIM_CLIENT_H

#include "Listenable.hpp"
#include "Server.hpp"

#include <arpa/inet.h>

namespace bim {
/**
 * A client object represent a connection of a client through socket.
 */
class Client: public Listenable
{
    public:
        Client();
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

        virtual void onIn();
        virtual void onOut();
        virtual void onErr();

    protected:
        struct sockaddr_in6 _address;
};
} // /bim

#endif

