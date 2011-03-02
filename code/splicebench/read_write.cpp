#include <iostream>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <cstring>

#include "../scoped_timer.h"

#define TEST_FAILURE(x) if(x < 0) {\
	std::cerr << __FILE__ \
			  << ":" << __LINE__ \
			  << ":" << #x << " : " << \
			  strerror(errno) << std::endl;\
	}

const size_t BLOCK_SIZE = 4 * 4096;

int main()
{
  int fd_in;
  int fd_out;
  fd_in = open("file1rw", O_RDONLY);
  TEST_FAILURE(fd_in);

  fd_out = creat("file2rw", S_IRWXU);
  TEST_FAILURE(fd_out);

  int rv = 0;
  {
      char buffer[BLOCK_SIZE];
      scoped_timer timer(std::cout);
    do {
      TEST_FAILURE((rv = read(fd_in, buffer, BLOCK_SIZE)));
      TEST_FAILURE(write(fd_out, buffer, rv));
    } while (rv > 0); 
  }

  TEST_FAILURE(rv);

  return 0;
}
