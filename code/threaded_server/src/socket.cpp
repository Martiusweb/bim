/// Author : Paul ADENOT <paul@paul.cx>
/// License : WTFPL <http://sam.zoy.org/wtfpl/>


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "macros.hpp"

void init_sockaddr(struct sockaddr_in *name, const char* hostname, int port)
{
  struct hostent *hostinfo;

  name->sin_family = AF_INET;
  name->sin_port = htons(port);

  hostinfo = gethostbyname(hostname);

  TEST_FAILURE(hostinfo == NULL);

  name->sin_addr = *(struct in_addr *) hostinfo->h_addr_list[0];
}

void bind_socket(int fd, const char* hostname, int port)
{
  struct sockaddr_in addr;

  init_sockaddr(&addr, hostname, port);

  TEST_FAILURE(bind(fd, (struct sockaddr *) &addr, sizeof(addr)));
}

void close_socket(int fd)
{
  shutdown(fd, SHUT_RD);
  close(fd);
}

int send_socket(int fd, const char *buffer, size_t tailleBuf)
{
  int count = 0;

  // MSG_NOSIGNAL is needed if the client closes its connection when we are
  // about to send back data. This prevent SIGPIPE.
  count = send(fd, buffer, tailleBuf, MSG_NOSIGNAL);

  TEST_FAILURE(count == -1);

  return count;
}


