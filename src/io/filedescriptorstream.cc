#include "filedescriptorstream.hh"

#include "io.hh"
#include "inputhandler.hh"
#include "outputhandler.hh"
#include "iostatemonitor.hh"

#include <iostream>

#include <unistd.h>
#include <cassert>

using namespace rain;
using namespace io;

FileDescriptorStream::FileDescriptorStream(int fd)
  : d_fileDescriptor(fd),
    d_readable(RWS_NO),
    d_writable(RWS_NO)
{
  IoStateMonitor::getInstance().registerFileDescriptor(d_fileDescriptor, this);
}

FileDescriptorStream::~FileDescriptorStream()
{
  IoStateMonitor::getInstance().unregisterFileDescriptor(d_fileDescriptor);
  closeInputHandler();
  closeOutputHandler();
}
      
void FileDescriptorStream::connectInputHandler(std::shared_ptr<InputHandler> inputHandler)
{
  d_inputHandler = inputHandler;

  // Update the inputhandler with the current state on connecting.
  if (d_readable.load() == RWS_YES) {
    readableEvent();
  }
}

void FileDescriptorStream::connectOutputHandler(std::shared_ptr<OutputHandler> outputHandler)
{
  d_outputHandler = outputHandler;

  // Update the inputhandler with the current state on connecting.
  if (d_writable.load() == RWS_YES) {
    writableEvent();
  }
}

void FileDescriptorStream::ioStateChanged(int fd, uint32_t stateMask)
{
  if (d_fileDescriptor != fd) {
    return;
  }

  if (stateMask & io::READ_BIT) {
    std::cout << "FileDescriptorStream::ioStateChanged (read event) d_readable=" << d_readable.load() << ".\n";
    
    // Only ever generate an event if we are sure that we changed the
    // readable value from false to true.
    while (true) {
      size_t expected = RWS_NO;
      if (d_readable.compare_exchange_strong(expected, RWS_YES)) {
	readableEvent();
      } else if (expected != RWS_YES) {
	expected = RWS_SESSION;
	if (!d_readable.compare_exchange_strong(expected, RWS_SESSION_YES) && expected != RWS_SESSION_YES) {
	  // Something changed between our two atomic checks, so try again.
	  continue;
	}
      }
      break;
    }
  }

  if (stateMask & io::WRITE_BIT) {
    while (true) {
      size_t expected = RWS_NO;
      if (d_writable.compare_exchange_strong(expected, RWS_YES)) {
	writableEvent();
      } else if (expected != RWS_YES) {
	expected = RWS_SESSION;
	if (!d_writable.compare_exchange_strong(expected, RWS_SESSION_YES) && expected != RWS_SESSION_YES) {
	  // Something changed between our two atomic checks, so try again.
	  continue;
	}
      }
      break;
    }
  }
}

bool FileDescriptorStream::isReadable() const
{
  return (d_readable.load() == RWS_YES);
}

bool FileDescriptorStream::isWritable() const
{
  return (d_writable.load() == RWS_YES);
}

bool FileDescriptorStream::beginRead()
{
  std::cout << "FileDescriptorStream::beginRead(): d_readable=" << d_readable.load() << ".\n";  
  size_t expected = RWS_YES;
  return d_readable.compare_exchange_strong(expected, RWS_SESSION);
}

void FileDescriptorStream::endRead(size_t bytesRead, bool drained)
{
  std::cout << "FileDescriptorStream::endRead(): d_readable=" << d_readable.load() << ".\n";
  
  size_t expected = RWS_SESSION;
  
  if (drained) {
    if (d_readable.compare_exchange_strong(expected, RWS_NO)) {
      return;
    }
  } else {
    if (d_readable.compare_exchange_strong(expected, RWS_YES)) {
      readableEvent();
      return;
    }
  }

  expected = RWS_SESSION_YES;
  if (d_readable.compare_exchange_strong(expected, RWS_YES)) {
    readableEvent();
    return;
  }
  
  assert(0 && "endRead() called without a beginRead()");
}

bool FileDescriptorStream::beginWrite()
{
  size_t expected = RWS_YES;
  return d_writable.compare_exchange_strong(expected, RWS_SESSION);
}

void FileDescriptorStream::endWrite(size_t bytesWritten, bool full)
{
  size_t expected = RWS_SESSION;
  
  if (full) {
    if (d_readable.compare_exchange_strong(expected, RWS_NO)) {
      return;
    }
  } else {
    if (d_readable.compare_exchange_strong(expected, RWS_YES)) {
      readableEvent();
      return;
    }
  }

  expected = RWS_SESSION_YES;
  if (d_readable.compare_exchange_strong(expected, RWS_YES)) {
    readableEvent();
    return;
  }
  
  assert(0 && "endRead() called without a beginRead()");
}

int FileDescriptorStream::inputFileDescriptor() const
{
  return d_fileDescriptor;
}
      
int FileDescriptorStream::read(char *buffer, size_t length)
{
  return ::read(d_fileDescriptor, buffer, length);
}

int FileDescriptorStream::outputFileDescriptor() const
{
  return d_fileDescriptor;  
}
      
int FileDescriptorStream::write(char const *buffer, size_t length)
{
  return ::write(d_fileDescriptor, buffer, length);
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
  std::cout << "FileDescriptorStream::readableEvent() d_readable=" << d_readable.load() << ".\n";
  
  auto h = d_inputHandler;
  if (h) {
    h->onInputReadable();
  }
}

void FileDescriptorStream::writableEvent()
{
  auto h = d_outputHandler;
  if (h) {
    h->onOutputWritable();
  }
}
