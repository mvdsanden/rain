#ifndef __INC_RAIN_CORE_TASK_HH__
#define __INC_RAIN_CORE_TASK_HH__

#include <mutex>
#include <condition_variable>
#include <stdexcept>

namespace rain {
  namespace core {

    class TaskCompletionHandler;
    
    /**
     *  TODO: figure out life time!
     */
    class Task {

      enum State {
	SCHEDULED = 0,
	RUNNING = 1,
	DONE = 2,
	CANCELED = 3
      };

    protected:

      virtual void operator()() = 0;	
      
    public:

      Task();

      ~Task();
      
      void run();

      void reset();

      /**
       *  This cancels the task.
       */
      void cancel();

      /**
       *  \return true when the task is running or expected to execute in the future.
       */
      bool alive() const;

      /**
       *  \return true when task was canceled.
       */
      bool canceled() const;
      
      /**
       *  Waits until the task is done or canceled.
       */
      void wait();

      /**
       *  Sets a handler that is called on completion of the task (by being done or canceled).
       */
      void setCompletionHandler(TaskCompletionHandler *handler);
      
    protected:

      /**
       *  Should be called by the implementing task to signal
       *  that the task is done.
       */
      void done();
      
    private:

      State d_state;
      
      std::mutex d_mutex;
      std::condition_variable d_cond;

      TaskCompletionHandler *d_completionHandler;
      
    };

    class FunTask
      : public Task
    {
    public:

      /**
       *  Should return true when done.
       */
      typedef std::function<bool ()> Fun;

      FunTask(Fun const &fun)
	: d_fun(fun)
      {
      }

    protected:

      virtual void operator()()
      {
	if (!d_fun || d_fun()) {
	  done();
	}
      }
      
    private:

      Fun d_fun;
      
    };
    
  }
}

#endif // __INC_RAIN_CORE_TASK_HH__
