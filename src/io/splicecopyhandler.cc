#include "splicecopyhandler.hh"

SpliceCopyHandler::SpliceCopyHandler()
  : d_inputReadable(false),
    d_outputWritable(false),
    d_taskRunning(false)
{
}

SpliceCopyHandler::~SpliceCopyHandler()
{
}

void SpliceCopyHandler::onInputClosed()
{
}

void SpliceCopyHandler::onInputReadable()
{
  tryCopy();
}

void SpliceCopyHandler::connectSource(std::weak_ptr<Source> const &source)
{
  d_source = source;
}

void SpliceCopyHandler::onOutputClosed()
{
}

void SpliceCopyHandler::onOutputWritable()
{
  tryCopy();
}

void SpliceCopyHandler::connectSink(std::weak_ptr<Sink> const &sink)
{
  d_sink = sink;
}

/// TODO: The input and output handlers need to also be notified when
///       a sink or source stops being readable or writable!!!!!!!!!!
/// Maybe add third (unknown) state to d_readable/d_writable, so we can know if anything changed in the mean time?
void SpliceCopyHandler::onTaskCompleted(Task &task, TaskState state)
{
  if (!d_taskRunning) {
    return;
  }

  d_taskRunning = false;
  
  if (d_inputReadable && d_outputWritable) {
    tryCopy();
  }
}

void SpliceCopyHandler::tryCopy()
{
  auto source = d_source.lock();
  auto sink = d_sink.lock();

  if (!source || !sink) {
    return;
  }

  if (!source->beginRead()) {
    return;
  }

  if (!sink->beginRead()) {
    source->endRead();
  }

}

void SpliceCopyHandler::copy()
{
  d_inputReadable = false;
  d_outputWritable = false;

}
