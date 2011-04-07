/**
 * Here is how we used to design a server.
 * The protocol is stupid : it just answer the string "plop\n"
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>


/* The answer of the server */
static const char* SERVER_ANSWER = "plop\n";

/* Server socket port */
static const uint16_t SERVER_PORT = 7000;

/* Maximum clients allowed */
static const int MAX_CLIENTS = 1;

static const int BUFFER_SIZE = 256;

int main()
{
    /* Socket descriptor */
    int server_socket = -1, client_socket = -1, answer_ln = -1, tmp = 0;
    /* Socket parameters structure, sockaddr_in is used for INET connections */
    struct sockaddr_in server_addr = {0}, client_addr = {0};
    /* Read buffer */
    char buffer[BUFFER_SIZE];

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

    /* Bind the socket to a local address : the socket will be associated to
       some interface, and thus accessible from the outside */

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
        perror("Can't bind the socket ");
        exit(errno);
    }

    printf("Socket bound !\n");

    /* We tell to the system that we want to listen on the interface for
       incomming connections */
    if(listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Can't listen on this interface ");
        exit(errno);
    }

    /* This call means we accept connections : the system creates a new socket
     * in order to let the server socket accept other connections */
    tmp = sizeof(client_addr);
    if((client_socket = accept(server_socket, (struct sockaddr*) &client_addr,
                    &tmp)) < 0) {
        perror("Can't accept client ");
        exit(errno);
    }

    printf("Client connected with IP : %s (socket : %d)\n",
            inet_ntoa(client_addr.sin_addr), client_socket);

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

    /* Say good bye when leaving ! */
    close(client_socket);
    close(server_socket);
    printf("Good Bye !\n");

    return 0;
}

