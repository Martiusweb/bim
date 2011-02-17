/// Author : Paul ADENOT <paul@paul.cx>
/// License : WTFPL <http://sam.zoy.org/wtfpl/>


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <libgen.h>
#include <arpa/inet.h>


#include "server.hpp"


// The default port the server binds to.
// This shall be 80 for a real web server, but
// during my testing, I run another webserver in
// parallel.
const int DEFAULT_PORT = 8080;

int main(int argc, char **argv)
{
  struct server_infos infos = {-1, DEFAULT_PORT, 0, "0.0.0.0", "."};

  if(arguments(argc, argv, &infos) == -1)
  {
    usage();
  }

  server_init(&infos);
  start(&infos);

  return 0;
}
