#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char** argv)
{
    int* clients;
    int max_clients = 0, i = 0;
    struct sockaddr_in server_addr;

    if(argc < 2) {
        perror("Too few args\n");
        exit(0);
    }
    sscanf(argv[1], "%i", &max_clients);

    printf("ready for %d clients !\n", max_clients);
    clients = malloc(max_clients*sizeof(int));

    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = 7001;

    for(i = 0; i < max_clients; ++i) {
        clients[i] = socket(AF_INET, SOCK_STREAM, 0);
        if(connect(clients[i], (struct sockaddr*) &server_addr,
                    sizeof(server_addr))
                <0) {
            --i;
        }
        printf("%d clients\n", i);
    }

    scanf("exit\n");
    for(i = 0; i < max_clients; ++i) {
        close(clients[i]);
    }

    free(clients);
    return 0;
}

