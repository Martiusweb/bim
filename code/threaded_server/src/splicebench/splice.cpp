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
  int pipe_des[2];
  fd_in = open("file1", O_RDONLY);
  TEST_FAILURE(fd_in);

  TEST_FAILURE(pipe(pipe_des));


  fd_out = creat("file2", S_IRWXU);
  TEST_FAILURE(fd_out);

  TEST_FAILURE(posix_fadvise(fd_in, 0,0,POSIX_FADV_SEQUENTIAL | POSIX_FADV_WILLNEED));

  int rv = 0;
  {
      scoped_timer timer(std::cout);
    do {
      rv = splice(fd_in, 0, pipe_des[1], 0, BLOCK_SIZE, SPLICE_F_MORE|SPLICE_F_MOVE);
      rv = splice(pipe_des[0], 0, fd_out, 0, rv, SPLICE_F_MORE|SPLICE_F_MOVE);
    } while (rv > 0); 
  }

  TEST_FAILURE(rv);

  return 0;
}
