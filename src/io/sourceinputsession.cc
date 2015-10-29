#include "sourceinputsession.hh"

#include "source.hh"
#include "exceptions/errnoexception.hh"

#include <iostream>

using namespace rain;
using namespace io;

SourceInputSession::SourceInputSession(std::weak_ptr<Source> const &source)
  : d_source(source),
    d_drained(false)
{
  auto src = d_source.lock();

  std::cout << "SourceInputSession(): src=" << !!src << ".\n";
  
  if (!src || !src->beginRead()) {
    std::cout << "SourceInputSession(): invalidated!\n";
    d_source.reset();
    return;
  }
}

SourceInputSession::~SourceInputSession()
{
  auto src = d_source.lock();

  if (!src) {
    return;
  }

  std::cout << "SourceInputSession: calling endRead(" << d_bytesRead << ", " << d_drained << ").\n";
  
  src->endRead(d_bytesRead, d_drained);
}
      
int SourceInputSession::fileDescriptor() const
{
  auto src = d_source.lock();

  if (!src) {
    return -1;
  }

  return src->inputFileDescriptor();
}

size_t SourceInputSession::read(char *buffer, size_t length)
{
  auto src = d_source.lock();

  if (!src) {
    return 0;
  }

  int ret = src->read(buffer, length);

  if (ret < 0) {
    if (errno == EAGAIN || errno == EINTR) {
      d_drained = true;      
      return 0;
    }

    throw exceptions::ErrnoException(errno);
  }

  d_bytesRead += ret;
  
  return static_cast<size_t>(ret);
}

bool SourceInputSession::isValid() const
{
  return !d_source.expired() && !d_drained;
}
