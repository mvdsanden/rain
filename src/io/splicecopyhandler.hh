#ifndef __INC_RAIN_IO_SPLICE_COPY_HANDLER_HH__
#define __INC_RAIN_IO_SPLICE_COPY_HANDLER_HH__

#include "copyhandler.hh"
#include "core/taskcompletionhandler.hh"

namespace rain {
  namespace io {

    class Task;
    
    class CLASS_PUBLIC SpliceCopyHandler
      : public CopyHandler,
	public TaskCompletionHandler
    {
    public:

      SpliceCopyHandler();

      ~SpliceCopyHandler();
      
      virtual void onInputClosed();

      virtual void onInputReadable();

      virtual void connectSource(std::weak_ptr<Source> const &source);

      virtual void onOutputClosed();

      virtual void onOutputWritable();

      virtual void connectSink(std::weak_ptr<Sink> const &sink);

      virtual void onTaskCompleted(Task &task, TaskState state);
      
    private:

      void tryCopy();
      void copy();
      
      std::atomic<bool> d_inputReadable;
      std::atomic<bool> d_outputWritable;
      std::atomic<bool> d_taskRunning;
      
      std::weak_ptr<Source> d_source;
      std::weak_ptr<Sink> d_sink;
      
    };
    
  }
}

#endif // __INC_RAIN_IO_SPLICE_COPY_HANDLER_HH__
