#ifndef __INC_RAIN_IO_SOURCE_HH__
#define __INC_RAIN_IO_SOURCE_HH__

#include "util/visibility.hh"

#include <memory>

namespace rain {
  namespace io {

    class InputHandler;
    
    class CLASS_PUBLIC Source {
    public:

      /**
       *  Connects an input handler object to the source.
       *
       *  The input handler will receive readable and close events from the source.
       */
      virtual void connectInputHandler(std::shared_ptr<InputHandler> inputHandler) = 0;
      
    };
    
  }
}

#endif // __INC_RAIN_IO_SOURCE_HH__
