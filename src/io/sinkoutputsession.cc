#include "sinkoutputsession.hh"

#include "sink.hh"
#include "exceptions/errnoexception.hh"

using namespace rain;
using namespace io;

SinkOutputSession::SinkOutputSession(std::weak_ptr<Sink> const &sink)
  : d_sink(sink),
    d_full(false)
{
  auto snk = d_sink.lock();

  if (!snk || !snk->beginWrite()) {
    d_sink.reset();
    return;
  }
}

SinkOutputSession::~SinkOutputSession()
{
  auto snk = d_sink.lock();

  if (!snk) {
    return;
  }

  snk->endWrite(d_bytesWritten, d_full);
}
      
int SinkOutputSession::fileDescriptor() const
{
  auto snk = d_sink.lock();

  if (!snk) {
    return -1;
  }

  return snk->outputFileDescriptor();
}

size_t SinkOutputSession::write(char *buffer, size_t length)
{
  auto snk = d_sink.lock();

  if (!snk) {
    return 0;
  }

  int ret = snk->write(buffer, length);

  if (ret < 0) {
    if (errno == EAGAIN || errno == EINTR) {
      d_full = true;      
      return 0;
    }

    throw exceptions::ErrnoException(errno);
  }

  return static_cast<size_t>(ret);
}

bool SinkOutputSession::isValid() const
{
  return !d_sink.expired() && !d_full;
}
