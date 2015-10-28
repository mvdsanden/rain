#ifndef __INC_RAIN_IO_IO_STATE_CHANGED_HANDLER_HH__
#define __INC_RAIN_IO_IO_STATE_CHANGED_HANDLER_HH__

#include <cstdint>

namespace rain {
  namespace io {

    class IoStateChangedHandler {
    public:

      virtual void ioStateChanged(int fd, uint32_t stateMask) = 0;
      
    };
    
  }
}

#endif // __INC_RAIN_IO_IO_STATE_CHANGED_HANDLER_HH__
