#include <iostream>
#include "thread_pool.h"
#include <assert.h>

int main()
{
  char plop = 1;
  char plop2 = 2;
  bim::ThreadPool pool; 
  char plop3 = 3;

  char* plop4 = new char;
  char* plop5 = new char;
  char* plop6 = new char;

  std::cerr << std::hex << "&plop:" << (void*)&plop << std::endl;
  std::cerr << std::hex << "&plop2:"  << (void*)&plop2 << std::endl;
  std::cerr << std::hex << "&plop3:" << (void*)&plop3 << std::endl;

  std::cerr << std::hex << "plop4:" << (void*)plop4 << std::endl;
  std::cerr << std::hex << "plop5:"  << (void*)plop5 << std::endl;
  std::cerr << std::hex << "plop6:" << (void*)plop6 << std::endl;

  plop = plop2;
  plop3 = 4;

  epoll_event event;

  event.data.ptr = &pool;

  int16_t instance;
  bim::ThreadPool* c = &pool;
  std::cerr << std::hex << "c:" << c << std::endl;

  event.data.ptr = (void*)((uintptr_t) &pool | 1);

  instance = (uintptr_t)event.data.ptr & 1;
  c = (bim::ThreadPool*) ((uintptr_t) c & (uintptr_t)~1);

  std::cerr << "instance : " << instance << std::endl;
  std::cerr << std::hex << "c&~1:" << c <<  std::endl;

  assert(c == &pool);

  uintptr_t fd;

  std::cerr << "size pointer:" << sizeof(bim::ThreadPool*) << std::endl;
  std::cerr << "size uintptr_t:" << sizeof((uintptr_t)event.data.ptr) << std::endl;
  std::cerr << "uintptr_t:" << sizeof(uintptr_t) << std::endl;
  std::cerr << "~1 : " << std::hex << (uintptr_t)~1 << std::endl;

  return 0;
}
