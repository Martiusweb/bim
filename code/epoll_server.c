/**
 * A simple server implemented using e-poll.
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sys/epoll.h>

/*** Configuration ***/
/* Server port */
static const uint16_t SERVER_PORT = 7001;
/* Max clients */
static const int MAX_CLIENTS = 1000;
/* Max epoll events that can be caught
 * TODO explain how to choose this value (currently *rand*) */
static const int MAX_EPOLL_EVENTS = 64;
/* Max epoll events processed per loop
 * TODO explain this one too */
static const int MAX_EPOLL_EVENTS_PER_LOOP = 16;

/**
 * Sets the socket described by fd as non-blocking.
 * Returns the socket fd or -1 in case of error.
 */
static int socket_set_nonblocking(int fd)
{
    int socket_flags = 0;
    /* Reuse address : tells the kernel that even if this port is busy (in the
       TIME_WAIT state), go ahead and reuse it anyway. Avoid "Already in use"
       error when restarting the program.  */
    if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &socket_flags, sizeof(int))
            < 0) {
        return -1;
    }

    socket_flags = 0;
    if((socket_flags = fcntl(fd, F_GETFL, 0)) == -1) {
        return -1;
    }

    if(fcntl(fd, F_SETFL, socket_flags|O_NONBLOCK) == -1) {
        return -1;
    }

    return fd;
}

/**
 * Create a socket ready to accept connections.
 * @param port listening port
 * @param max_clients maximum of clients allowed by the server
 * Returns a file descriptor or -1 in case of error.
 */
static int create_server(uint16_t port, int max_clients)
{
    int socket_server, epollq;
    struct sockaddr_in addr_server = {0}; 

    /* Initialize the server socket */
    socket_server = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_server == -1) {
        perror("Server socket can not be created ");
        return -1;
    }

    addr_server.sin_family = AF_INET;
    addr_server.sin_port = htons(port);
    addr_server.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(socket_server, (struct sockaddr*) &addr_server,
                sizeof(addr_server)) != 0) {
        close(socket_server);
        perror("Can not bind the socket ");
        return -1;
    }

    if(listen(socket_server, max_clients) < 0) {
        close(socket_server);
        perror("Can not listen on this interface ");
        return -1;
    }

    return socket_server;
}

/**
 * Add the socket to the I/O descriptors watched by our epoll structure.
 * @param epollq epoll file descriptor
 * @param socket_server server socket to watch
 * @param mode epoll_ctl mode (EPOLL_CTL_ADD or EPOLL_CTL_MOD)
 * Returns the epoll fd or -1 in case of error
 */
static int epoll_watch_in(int epollq, int socket_server, int mode =
        EPOLL_CTL_ADD)
{
    struct epoll_event event;

    /* Nb events EPOLLHUP and EPOLLERR are impicitly set */
    event.events = EPOLLIN /* ready to read */ | EPOLLET /* Edge Triggered */;
    event.data.fd = socket_server;

    if(epoll_ctl(epollq, mode, socket_server, &event) == -1) {
        return -1;
    }

    return epollq;
}

/**
 * Add the socket to the I/O descriptors watched by our epoll structure.
 * @param epollq epoll file descriptor
 * @param socket_client client socket to watch
 * @param mode epoll_ctl mode (EPOLL_CTL_ADD or EPOLL_CTL_MOD)
 * Returns the epoll fd or -1 in case of error
 */
static int epoll_watch_inout(int epollq, int socket_client, int mode =
        EPOLL_CTL_ADD)
{
    struct epoll_event event;

    /* @see epoll_watch_in */
    event.events = EPOLLIN | EPOLLOUT /* ready to write */ | EPOLLET;
    event.data.fd = socket_client;

    if(epoll_ctl(epollq, mode, socket_client, &event) == -1) {
        return -1;
    }
    return epollq;
}

/**
 * Remove the file descriptor fd of those watched by epollq.
 * @param epollq epoll descriptor
 * @param fd file descriptor
 * Returns the epoll fd or -1 in case of error
 */
static int epoll_unwatch(int epollq, int fd)
{
    /* We could be bitches and use a static structure */
    struct epoll_event event;
    if(epoll_ctl(epollq, EPOLL_CTL_DEL, fd, &event) == -1) {
        return -1;
    }
    return epollq;
}

/**
 * Enter the program, create the server socket.
 */
int main()
{
    int socket_server = 0, socket_client = 0, epollq = 0, nb_events = 0,
        read = 0, i = 0;
    socklen_t addr_ln = 0;
    struct epoll_event epoll_event, events[MAX_EPOLL_EVENTS_PER_LOOP];
    /* TODO it may be good to keep the user address somewhere... */
    struct sockaddr_in addr_client = {0};

    /* For testing purpose : implementation quite stupid of a dummy protocol */
    int BUFFER_SIZE = 256;
    char buffer[BUFFER_SIZE];

    if((socket_server = create_server(SERVER_PORT, MAX_CLIENTS)) == -1) {
        perror("Can not create or bind or listen on the server socket ");
        exit(errno);
    }
    printf("Listening to %d\n", SERVER_PORT);

    if((epollq = epoll_create(MAX_EPOLL_EVENTS)) == -1) {
        close(socket_server);
        perror("Can not create epoll object ");
        exit(errno);
    }
    printf("Epoll ready to wait for %d events !\n", MAX_EPOLL_EVENTS);

    if(epoll_watch_in(epollq, socket_server) == -1) {
        close(epollq);
        close(socket_server);
        perror("Can not add the server socket file descriptor to epoll ");
        exit(errno);
    }

    /* Nao, our event loop */
    for(;;)
    {
        /* Blocking wait, epoll will store at least */
        nb_events = epoll_wait(epollq, events, MAX_EPOLL_EVENTS_PER_LOOP, -1);

        for(i = 0; i < nb_events; ++i)
        {
            /* error on a client socket */
            if(events[i].data.fd != socket_server &&
                    (events[i].events & (EPOLLHUP|EPOLLERR))) {
                perror("Connection with client closed\n");
                epoll_unwatch(epollq, events[i].data.fd);
                close(events[i].data.fd);
            }
            else if(events[i].data.fd == socket_server && (events[i].events &
                        EPOLLIN)) {
                if((socket_client = accept(socket_server, (struct sockaddr*)
                                &addr_client, &addr_ln)) > 0) {

                    if(socket_set_nonblocking(socket_client) == -1) {
                        close(socket_client);
                        perror("Can not set the client non blocking\n");
                    }
                    else if(epoll_watch_in(epollq, socket_client) == -1) {
                        close(socket_client);
                        perror("Can not watch the client with epoll\n");
                    }
                }
            }
            else {
                if(events[i].events & EPOLLIN) {
                    read = recv(events[i].data.fd, buffer, BUFFER_SIZE,
                            MSG_DONTWAIT);
                    epoll_watch_inout(epollq, events[i].data.fd,
                            EPOLL_CTL_MOD);
                    buffer[read] = 0;
                    printf("Read from socket \"%s\"\n", buffer);
                }
                if(events[i].events & EPOLLOUT) {
                    send(events[i].data.fd, "plop\n", 5,
                            MSG_DONTWAIT|MSG_NOSIGNAL);
                    epoll_unwatch(epollq, events[i].data.fd);
                    close(events[i].data.fd);
                }
            }
        }
    }

    close(epollq);
    close(socket_server);
    printf("Good bye !\n");
    return 0;
}

