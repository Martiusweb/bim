/// Author : Paul ADENOT <paul@paul.cx>
/// License : WTFPL <http://sam.zoy.org/wtfpl/>

#ifndef MACROS_HPP
#define MACROS_HPP

#include <iostream>
#include <cerrno>

#define dump_var(x) {std::cerr << #x << ":" << x << ":" << std::endl;}
#define LOG std::cerr
#define TEST_FAILURE(x) if(x) {\
	std::cerr << __FILE__ \
			  << ":" << __LINE__ \
			  << ":" << #x << " : " << \
			  strerror(errno) << std::endl;\
	}


#endif
