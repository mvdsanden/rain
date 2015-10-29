#ifndef __INC_RAIN_IO_SOURCE_HH__
#define __INC_RAIN_IO_SOURCE_HH__

#include "util/visibility.hh"

#include <memory>

namespace rain {
  namespace io {

    class InputHandler;
    
    class CLASS_PUBLIC Source {
      friend class SourceInputSession;
    public:

      /**
       *  Connects an input handler object to the source.
       *
       *  The input handler will receive readable and close events from the source.
       */
      virtual void connectInputHandler(std::shared_ptr<InputHandler> inputHandler) = 0;

      /**
       *  \return true if the source is readable without blocking.
       */
      virtual bool isReadable() const = 0;

    protected:
      
      /**
       *  Must be called before anyting is read from the source and it was not
       *  an object internal function that was used.
       *
       *  This starts a read session, that must be closed by a call to endRead().
       *
       *  \return false when the source was not readable, no session was started in this case!
       *
       *  Note: no reading should be done when this method returns false!
       */
      virtual bool beginRead() = 0;

      /**
       *  Must be called after reading from the source to close a beginRead().
       *
       *  \param bytesRead the number of bytes read since the call to beginRead().
       *  \param drained set to true to indicate that the read returned EAGAIN or similar.
       */
      virtual void endRead(size_t bytesRead, bool drained) = 0;

      virtual int inputFileDescriptor() const = 0;
      
      virtual int read(char *buffer, size_t length) = 0;
      
    };
    
  }
}

#endif // __INC_RAIN_IO_SOURCE_HH__
