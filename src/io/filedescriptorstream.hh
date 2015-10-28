#ifndef __INC_RAIN_FILE_DESCRIPTOR_STREAM_HH__
#define __INC_RAIN_FILE_DESCRIPTOR_STREAM_HH__

#include "stream.hh"
#include "iostatechangedhandler.hh"

namespace rain {
  namespace io {

    class CLASS_PRIVATE FileDescriptorStream
      : public Stream,
	public IoStateChangedHandler
    {
    public:

      FileDescriptorStream(int fd);

      ~FileDescriptorStream();
      
      virtual void connectInputHandler(std::shared_ptr<InputHandler> inputHandler);

      virtual void connectOutputHandler(std::shared_ptr<OutputHandler> outputHandler);

      virtual void ioStateChanged(int fd, uint32_t stateMask);
      
    private:

      void closeInputHandler();

      void closeOutputHandler();

      void readableEvent();

      void writableEvent();
      
      int d_fileDescriptor;

      uint32_t d_stateMask;

      std::shared_ptr<InputHandler> d_inputHandler;

      std::shared_ptr<OutputHandler> d_outputHandler;

    };

  }
}

#endif // __INC_RAIN_FILE_DESCRIPTOR_STREAM_HH__
