#ifndef __INC_RAIN_IO_INPUT_HANDLER_HH__
#define __INC_RAIN_IO_INPUT_HANDLER_HH__

#include "util/visibility.hh"

#include <memory>

namespace rain {
  namespace io {

    class Source;
    
    class CLASS_PUBLIC InputHandler {
    public:

      /**
       *  This event should be called when the source to which the handler
       *  is connected is closed.
       */
      virtual void onInputClosed() = 0;

      /**
       *  This event should be called when the source to which the handler
       *  is connected has become readable.
       */
      virtual void onInputReadable() = 0;

      /**
       *  Connects a source to the input handler.
       *
       *  And input handler never owns the source, the source owns it usually.
       */
      virtual void connectSource(std::weak_ptr<Source> const &source) = 0;
      
    };

  }
}


#endif // __INC_RAIN_IO_INPUT_HANDLER_HH__
