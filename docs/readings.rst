Readings
########

Some threads or article I found that were usefull :

The Apache benchmarking tool
  http://httpd.apache.org/docs/2.0/programs/ab.html
The c10k problem
  http://www.kegel.com/c10k.html#nb.epoll
  There are a lot utra usefull links : this is a great summup
Epoll scalability web page
  http://lse.sourceforge.net/epoll/index.html
Progress on multi thread epoll support
  http://blogs.balabit.com/en/2010/11/07/progress-on-multi-thread-epoll-support/
  Talks about a specific case, but it may be generalized, right ?
Research paper describing the architecture (linked in c10k page)
  http://www.cs.rice.edu/~druschel/usenix99event.ps.gz
Splice : a system call which allow not to copy data in userspace
  http://en.wikipedia.org/wiki/Splice_%28system_call%29
0mQ : Datagram with transmission guarantees
  http://www.zeromq.org/
Sendfile() and splice() and perfs
  http://blog.superpat.com/2010/06/01/zero-copy-in-linux-with-sendfile-and-splice/

  Obviously, Linux man pages are a good start.
