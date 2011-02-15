/**
 * Here is how we used to design a server.
 * The protocol is stupid : it just answer the string "plop\n"
 * use -pthread flag to compile
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/epoll.h>

/* The answer of the server */
static const char* SERVER_ANSWER = "plop\n";

/* Server socket port */
static const uint16_t SERVER_PORT = 7000;

/* Maximum clients allowed */
static const int MAX_CLIENTS = 10;

static const int BUFFER_SIZE = 256;

static int socket_set_nonblocking(int socket)
{
    int socket_flags = 1;
    /* Reuse address : tells the kernel that even if this port is busy (in the
       TIME_WAIT state), go ahead and reuse it anyway. Avoid "Already in use"
       error when restarting the program.  */
    if(setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &socket_flags, sizeof(int))
            < 0) {
        return -1;
    }

    /* Get the flags currently set for the socket */
    socket_flags = 0;
    if((socket_flags = fcntl(socket, F_GETFL, 0)) == -1) {
        return -1;
    }

   if(fcntl(socket, F_SETFL, flags|O_NONBLOCK) == -1) {
        return -1;
   }

   return fd;
}
/**
 * Handle the client connected : wait for a message and answer
 */
static void* handle_client(void* arg)
{
    /* utility */
    int answer_ln, client_socket;

    /* Read buffer */
    char buffer[BUFFER_SIZE];

    client_socket = (int) arg;

    /* I wait to receive something */
    if(recv(client_socket, buffer, BUFFER_SIZE, 0) < 0) {
        perror("Can't receive ");
        exit(errno);
    }

    /* Send the answer, sure he will love it */
    answer_ln = strlen(SERVER_ANSWER);
    if(send(client_socket, SERVER_ANSWER, answer_ln, 0 /* No specific flag */)
        < 0) {
        perror("Message not sent ");
        exit(errno);
    }


    close(client_socket);
    pthread_exit(0);
}

int main()
{
    /* Socket descriptor */
    int server_socket = -1, client_socket = -1, epoll = -1, nb_events = 0,
        tmp = 0, i = 0;
    /* Socket parameters structure, sockaddr_in is used for INET connections */
    struct sockaddr_in server_addr = {0}, client_addr = {0};
    /* epoll event structure */
    struct epoll_event event, events[MAX_CLIENTS];
    /* Threads management */
    pthread_t thread;

    /*************************************************************************/
    printf("Hello !\n");
    
    /* Initializing the server */
    server_socket = socket(
        AF_INET, /* IPv4 */
        SOCK_STREAM, /* TCP */
        IPPROTO_TCP /* the protocol parameter is not used, the value is null */
    );

    if(!server_socket == -1) {
        perror("Can't create a socket. ");
        exit(errno);
    }

    printf("Socket created\n");

    if(socket_set_nonblocking(server_socket) == -1) {
        close(server_socket);
        printf("Can't set the server socket as non-blocking ");
        exit(errno);
    }

    printf("Socket options are OK\n");

    /* Bind the socket to a local address : the socket will be associated to
       some interface, and thus accessible from the outside */
    /* This is safer and recommended everywhere I read stuff about sockets,
       well... why not. I just think it's not necessary at all. */
    memset(server_addr, 0, sizeof(struct sockaddr_in));
    /* We are using a PF_INET socket */
    server_addr.sin_family = AF_INET; 
    /* Define the port, htons() converts values between host and network byte
       order (see man htons) */
    server_addr.sin_port = htons(SERVER_PORT);
    /* For each network interface of the computer, we have an ip, we use the
       const INADDR_ANY to allow connections from every interfaces */
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* Bind ! */
    if(bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)) != 0)
    {
        close(server_socket);
        perror("Can't bind the socket ");
        exit(errno);
    }

    printf("Socket bound !\n");

    /* We tell to the system that we want to listen on the interface for
       incomming connections */
    if(listen(server_socket, MAX_CLIENTS) < 0) {
        close(server_socket);
        perror("Can't listen on this interface ");
        exit(errno);
    }

    /* Creating the epoll object */
    if((epoll = epoll_create(MAX_CLIENTS)) == -1) {
        close(server_socket);
        printf("Can't create epoll object ");
        exit(errno);
    }

    /* Time to configure epoll events */
    /* Events filtered by epoll */
    event.events = EPOLLIN /* read() */ | EPOLLPRI /* urgent data */;
    /* note : EPOLLERR and EPOLLHUP are implicitly set */
    event.data.fd = server_socket;

    /* Add the server socket at fd watched by epoll */
    if(epoll_ctl(epoll, EPOLL_CTL_ADD, server_socket, &event) == -1) {
        close(epoll);
        close(server_socket);
        printf("Can't add the socket fd to epoll ");
        exit(errno);
    }

    /* This call means we accept connections : the system creates a new socket
     * in order to let the server socket accept other connections */
    for(;;)
    {
        /* We get events epoll collected for us here, max events = max clients.
           I'm not sure it's a good idea, but I think I'll do some tests to
           figure how to handle this configuration */
        nb_events = epoll_wait(epoll,
                events /* Structure were events are stored */,
                MAX_CLIENTS /* actually, it's the size of the events array */,
                -1 /* Timeout ? No wait !*/);

        /* Event loop */
        for(i = 0; i < nb_events, ++i)
        {
            /* event is an error on a client socket */
            if(events[i].data.fd != server_socket &&
                    (events[i].events & (EPOLLHUP | EPOLLERR))) {
                perror("Error detected in epoll event, a client is not fine !");
                /* I remove & close this socket from those managed by epoll */
                event.data.fd = events[i].data.fd;
                if(epoll_ctl(epoll, EPOLL_CTL_DEL, events[i].data.fd, &event) < 0) {
                    perror("Can't remove this broken client...");
                }
                close(events[i].data.fd);
            }
            /* Event is data ready to be read (hint) */
            else if(events[i].events & EPOLLIN) {
                if(events[i].data.fd = server_socket) {
                if((client_socket = accept(server_socket, (struct sockaddr*) & client_addr, &tmp)) < 0) {
                    perror("Can't accept client ");
                    exit(errno);
                }
                /* set client non blocking
                   add the client to epoll
                */
                }
                else {
                    // Process request
                }
            }

        }
        tmp = sizeof(client_addr);
        if((client_socket = accept(server_socket, (struct sockaddr*) &client_addr,
                        &tmp)) < 0) {
            perror("Can't accept client\n");
            /* we can't kill the server anymore ! */
        }

        printf("Client connected with IP : %s (socket : %d)\n",
                inet_ntoa(client_addr.sin_addr), client_socket);


        if(pthread_create(&thread, 0, &handle_client, (void*) client_socket)) {
            perror("Thread creation failed. Bouh\n");
        }
    }
    
    /* Say good bye when leaving ! */
    close(server_socket);
    printf("Good Bye !\n");

    return 0;
}

