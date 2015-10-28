#include "errnoexception.hh"

#include <string.h>

#include <iostream>

using namespace rain;
using namespace exceptions;

std::string _errorString(int errnum)
{
  char buf[256] = {}; // initializes to zeros.
  char *res = strerror_r(errnum, buf, 255);
  std::cout << "Throwing error: " << errnum << ": " << res << ".\n";
  return res;
}

ErrnoException::ErrnoException(int errnum)
  : std::runtime_error(_errorString(errnum))
{  
}
