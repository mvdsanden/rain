#ifndef __INC_RAIN_IO_SINK_OUTPUT_SESSION_HH__
#define __INC_RAIN_IO_SINK_OUTPUT_SESSION_HH__

#include <memory>

namespace rain {
  namespace io {

    class Sink;
    
    class SinkOutputSession
    {
    public:

      SinkOutputSession(std::weak_ptr<Sink> const &sink);

      ~SinkOutputSession();
      
      /**
       *  \return a file descriptor if the session has one associated with it, otherwise it returns -1.
       */
      int fileDescriptor() const;

      /**
       *
       */
      size_t write(char *buffer, size_t length);      

      bool isValid() const;
      
    private:

      std::weak_ptr<Sink> d_sink;
      
      size_t d_bytesWritten;
      bool d_full;
      
    };
    
  }
}


#endif // __INC_RAIN_IO_SINK_OUTPUT_SESSION_HH__
