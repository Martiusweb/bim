/// Author : Paul ADENOT <paul@paul.cx>
/// License : WTFPL <http://sam.zoy.org/wtfpl/>


#ifndef __H_SOCKET_H__
#define __H_SOCKET_H__


void init_sockaddr(struct sockaddr_in *name, const char *hostname, int port);
int bind_socket(int fd, const char *hostname, int port);
void close_socket(int fd);
int send_socket(int fd, const char *buffer, size_t tailleBuf);


#endif
