#include "filedescriptorstream.hh"

#include "io.hh"
#include "inputhandler.hh"
#include "outputhandler.hh"

using namespace rain;
using namespace io;

FileDescriptorStream::FileDescriptorStream(int fd)
  : d_fileDescriptor(fd),
    d_stateMask(0)
{
}

FileDescriptorStream::~FileDescriptorStream()
{
  closeInputHandler();
  closeOutputHandler();
}
      
void FileDescriptorStream::connectInputHandler(std::shared_ptr<InputHandler> inputHandler)
{
  d_inputHandler = inputHandler;
}

void FileDescriptorStream::connectOutputHandler(std::shared_ptr<OutputHandler> outputHandler)
{
  d_outputHandler = outputHandler;
}

void FileDescriptorStream::ioStateChanged(int fd, uint32_t stateMask)
{
  if (d_fileDescriptor != fd) {
    return;
  }

  d_stateMask = stateMask;

  if (stateMask & io::READ_BIT) {
    readableEvent();
  }

  if (stateMask & io::WRITE_BIT) {
    writableEvent();
  }
}

void FileDescriptorStream::closeInputHandler()
{
  auto h = d_inputHandler;
  if (h) {
    h->onInputClosed();
  }
  d_inputHandler.reset();
}

void FileDescriptorStream::closeOutputHandler()
{
  auto h = d_outputHandler;
  if (h) {
    h->onOutputClosed();
  }
  d_outputHandler.reset();
}

void FileDescriptorStream::readableEvent()
{
  auto h = d_inputHandler;
  if (h) {
    h->onInputReadable();
  }
  d_inputHandler.reset();  
}

void FileDescriptorStream::writableEvent()
{
  auto h = d_outputHandler;
  if (h) {
    h->onOutputWritable();
  }
  d_outputHandler.reset();
}
