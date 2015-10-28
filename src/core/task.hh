#ifndef __INC_RAIN_CORE_TASK_HH__
#define __INC_RAIN_CORE_TASK_HH__

#include <mutex>
#include <condition_variable>
#include <stdexcept>

namespace rain {
  namespace core {

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

      Task()
	: d_state(SCHEDULED)
      {
      }

      ~Task()
      {
	//std::unique_lock<std::mutex> lk(d_mutex);
	//if (d_state < DONE) {
	//  throw std::runtime_error("task deleted before it was finished");
	//}
      }
      
      void run()
      {
	std::unique_lock<std::mutex> lk(d_mutex);

	if (d_state != SCHEDULED) {
	  return;
	}
	
	d_state = RUNNING;

	lk.unlock();

	(*this)();

	lk.lock();

	if (d_state == RUNNING) {
	  d_state = SCHEDULED;
	}

	d_cond.notify_all();
      }

      void reset()
      {
	std::unique_lock<std::mutex> lk(d_mutex);

	if (d_state <= RUNNING) {
	  return;
	}

	d_state = SCHEDULED;
      }
      
      void cancel()
      {
	std::unique_lock<std::mutex> lk(d_mutex);

	while (d_state == RUNNING) {
	  d_cond.wait(lk);
	}

	d_state = CANCELED;	
	d_cond.notify_all();
      }
      
      bool alive() const
      {
	return d_state <= RUNNING;
      }
      
      void wait()
      {
	std::unique_lock<std::mutex> lk(d_mutex);
	while (d_state < DONE) {
	  d_cond.wait(lk);
	}
      }

    protected:

      void done()
      {
	std::unique_lock<std::mutex> lk(d_mutex);
	d_state = DONE;
	d_cond.notify_all();
      }

    private:

      State d_state;
      
      std::mutex d_mutex;
      std::condition_variable d_cond;
      
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
