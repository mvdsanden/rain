#ifndef __INC_RAIN_CORE_TASKCOMPLETIONHANDLER_HH__
#define __INC_RAIN_CORE_TASKCOMPLETIONHANDLER_HH__

#include "util/visibility.hh"

#include <functional>

namespace rain {
  namespace core {

    class Task;
    
    class CLASS_PUBLIC TaskCompletionHandler
    {
    public:

      enum TaskState {
	TASK_CANCELED = 0,
	TASK_DONE = 1,
      };
      
      virtual void onTaskCompleted(Task &task, TaskState state) = 0;

    };

    class CLASS_PUBLIC FunTaskCompletionHandler
      : public TaskCompletionHandler
    {
    public:
      
      typedef std::function<void (Task &, TaskState)> Callback;
      
      FunTaskCompletionHandler(Callback const &cb)
	: d_cb(cb)
	{
	}

      virtual void onTaskCompleted(Task &task, TaskState state)
      {
	if (d_cb) {
	  d_cb(task, state);
	}
      }
      
    private:

      Callback d_cb;
	
    };
    
  }
}

#endif // __INC_RAIN_CORE_TASKCOMPLETIONHANDLER_HH__
