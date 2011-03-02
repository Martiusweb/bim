#ifndef BIM_SERVER_H
#define BIM_SERVER_H

#include "Listenable.hpp"

namespace bim {
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
        Server(int port = 80, int max_clients = 1);
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

        virtual bool registerEventDispatcher(EventDispatcher& ed);

        virtual void onIn();
        virtual void onErr();

    protected:
        int _port;
        int _max_clients;
        int _sock_domain;
};
} // /bim

#endif

