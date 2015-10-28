#ifndef __INC_RAIN_ERRNOEXCEPTION_HH__
#define __INC_RAIN_ERRNOEXCEPTION_HH__

#include <stdexcept>

namespace rain {
  namespace exceptions {
    
    class ErrnoException : public std::runtime_error {
    public:

      ErrnoException(int errnum);

    };

  }
}

#endif // __INC_RAIN_ERRNOEXCEPTION_HH__
