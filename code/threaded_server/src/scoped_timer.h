/// Author : Paul ADENOT <paul@paul.cx>
/// License : WTFPL <http://sam.zoy.org/wtfpl/>

#include <iostream>
#include <boost/noncopyable.hpp>
#include <sys/time.h>


// output the time passed in a scope.
class scoped_timer
{
  public:
    scoped_timer(std::ostream& out)
      :output_(out)
    {
      gettimeofday(&start, NULL);
    }

    ~scoped_timer()
    {
      long seconds;
      long useconds;

      gettimeofday(&end, NULL);

      seconds  = end.tv_sec  - start.tv_sec;
      useconds = end.tv_usec - start.tv_usec;

      output_ << ((seconds) * 1000 + useconds/1000.) << std::endl;
    }
  private:
    std::ostream& output_;
    struct timeval start;
    struct timeval end;
};
