#ifndef __INC_RAIN_IO_SINK_HH__
#define __INC_RAIN_IO_SINK_HH__

#include "util/visibility.hh"

#include <memory>

namespace rain {
  namespace io {

    class OutputHandler;
    
    class CLASS_PUBLIC Sink {
    public:

      /**
       *  Connects an output handler object to the sink.
       *
       *  The output handler will receive writable and close events from the sink.
       */
      virtual void connectOutputHandler(std::shared_ptr<OutputHandler> outputHandler) = 0;
      
    };
    
  }
}

#endif // __INC_RAIN_IO_SINK_HH__
