#ifndef __INC_RAIN_IO_SINK_HH__
#define __INC_RAIN_IO_SINK_HH__

#include "util/visibility.hh"

#include <memory>

namespace rain {
  namespace io {

    class OutputHandler;
    
    class CLASS_PUBLIC Sink {
      friend class SinkOutputSession;
    public:

      /**
       *  Connects an output handler object to the sink.
       *
       *  The output handler will receive writable and close events from the sink.
       */
      virtual void connectOutputHandler(std::shared_ptr<OutputHandler> outputHandler) = 0;

      /**
       *  \return true when the sink is writable without blocking.
       */
      virtual bool isWritable() const = 0;

    protected:
      
      /**
       *  Must be called before anything is written to the sink and it was not
       *  an object internal function that was used.
       *
       *  This starts a write session, that must be closed by a call to endWrite().
       *
       *  \return false when the source was not writable, no session was started in this case!
       *
       *  Note: no writing should be done when this method returns false!
       */
      virtual bool beginWrite() = 0;

      /**
       *  Must be called after writing from the source to close a beginWrite().
       *
       *  \param bytesWritten the number of bytes written since the call to beginWrite().
       *  \param full set to true to indicate that the write returned EAGAIN or similar.
       */
      virtual void endWrite(size_t bytesWritten, bool full) = 0;

      virtual int outputFileDescriptor() const = 0;
      
      virtual int write(char const *buffer, size_t length) = 0;

      
    };
    
  }
}

#endif // __INC_RAIN_IO_SINK_HH__
