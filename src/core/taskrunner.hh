#ifndef __INC_RAIN_CORE_TASK_RUNNER_HH__
#define __INC_RAIN_CORE_TASK_RUNNER_HH__

#include <memory>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <list>

namespace rain {
  namespace core {

    class Task;
    
    class TaskRunner {
    public:

      virtual void submitTask(std::shared_ptr<Task> task) = 0;      
      
    };

    class DefaultTaskRunner
      : public TaskRunner
    {
    public:

      DefaultTaskRunner();

      ~DefaultTaskRunner();
      
      virtual void submitTask(std::shared_ptr<Task> task)
      {
	std::unique_lock<std::mutex> lk(d_bMutex);
	d_b.push_back(task);
	d_cond.notify_all();
      }

      void next(std::chrono::steady_clock::duration const &timeout = std::chrono::steady_clock::duration(0));

    private:

      std::mutex d_bMutex;
      std::condition_variable d_cond;
      
      std::list<std::shared_ptr<Task>> d_a;
      std::list<std::shared_ptr<Task>> d_b;
      
    };

  }
}

#endif // __INC_RAIN_CORE_TASK_RUNNER_HH__
