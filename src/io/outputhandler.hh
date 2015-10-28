#ifndef __INC_RAIN_IO_OUTPUT_HANDLER_HH__
#define __INC_RAIN_IO_OUTPUT_HANDLER_HH__

#include "util/visibility.hh"

#include <memory>

namespace rain {
  namespace io {

    class Sink;
    
    class CLASS_PUBLIC OutputHandler {
    public:

      /**
       *  This event should be called when the sink to which the handler
       *  is connected is closed.
       */
      virtual void onOutputClosed() = 0;

      /**
       *  This event should be called when the sink to which the handler
       *  is connected has become writable.
       */
      virtual void onOutputWritable() = 0;

      /**
       *  Connects a sink to the output handler.
       *
       *  And output handler never owns the sink, the sink owns it usually.
       */
      virtual void connectSink(std::weak_ptr<Sink> const &sink) = 0;
      
    };

  }
}


#endif // __INC_RAIN_IO_OUTPUT_HANDLER_HH__
