#ifndef __INC_RAIN_FILE_DESCRIPTOR_STREAM_HH__
#define __INC_RAIN_FILE_DESCRIPTOR_STREAM_HH__

#include "stream.hh"
#include "iostatechangedhandler.hh"

#include <atomic>

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

      virtual bool isReadable() const;

      virtual bool isWritable() const;

    protected:

      virtual bool beginRead();
      
      virtual void endRead(size_t bytesRead, bool drained);

      virtual bool beginWrite();

      virtual void endWrite(size_t bytesWritten, bool full);

      virtual int inputFileDescriptor() const;
      
      virtual int read(char *buffer, size_t length);
      
      virtual int outputFileDescriptor() const;
      
      virtual int write(char const *buffer, size_t length);
      
    private:

      void closeInputHandler();

      void closeOutputHandler();

      void readableEvent();

      void writableEvent();
      
      int d_fileDescriptor;

      enum RWState {
	// Not readable/writable.
	RWS_NO = 0,
	// Readable/writable.
	RWS_YES = 1,
	// In IO session (between beginRead/Write() and endRead/Write() calls.
	RWS_SESSION = 2,
	// We got a readable/writable event during a session.
	RWS_SESSION_YES = 3,
      };
      
      std::atomic<size_t> d_readable;
      
      std::atomic<size_t> d_writable;

      std::shared_ptr<InputHandler> d_inputHandler;

      std::shared_ptr<OutputHandler> d_outputHandler;

    };

  }
}

#endif // __INC_RAIN_FILE_DESCRIPTOR_STREAM_HH__
