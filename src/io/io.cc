#include "io.hh"

#include "exceptions/errnoexception.hh"

#include <unistd.h>

#include <sys/resource.h>


namespace rain {
  namespace io {

    size_t Io::getFileDescriptorLimit() const
    {
      rlimit l;
      int ret = getrlimit(RLIMIT_NOFILE, &l);
      
      if (ret == -1) {
	throw exceptions::ErrnoException(errno);
      }

      return l.rlim_cur;
    }

  }
}


