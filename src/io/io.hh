#ifndef __INC_RAIN_IO_HH__
#define __INC_RAIN_IO_HH__

#include "util/singleton.hh"

#include <cstddef>

namespace rain {
  namespace io {

    enum State {
      READ_BIT        = 0x00000001,
      PRIORITY_BIT    = 0x00000002,
      WRITE_BIT       = 0x00000004,
      ERROR_BIT       = 0x00000008,
      HANGUP_BIT      = 0x00000010,
      READ_HANGUP_BIT = 0x00002000,
    };

    class Io
      : public util::Singleton<Io>
    {
    public:

      /**
       *  \returns the maximum value a file descriptor can get.
       */
      size_t getFileDescriptorLimit() const;

    };
    
  }
}

#endif // __INC_RAIN_IO_HH__
