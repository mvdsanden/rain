#include "taskrunner.hh"

#include "task.hh"

using namespace rain;
using namespace core;

DefaultTaskRunner::DefaultTaskRunner()
{
}

DefaultTaskRunner::~DefaultTaskRunner()
{
}

void DefaultTaskRunner::next(std::chrono::steady_clock::duration const &timeout)
{
  // If queue a is empty, wait for queue b to have at least one
  // task, or the timeout expires.
  if (d_a.empty()) {
    std::unique_lock<std::mutex> lkB(d_bMutex);
    
    d_cond.wait_for(lkB, timeout, [&]()
		    {
		      return !d_b.empty();
		    });
    
    if (d_b.empty()) {
      return;
    }

    // If queue b has tasks, swap the queues.
    std::swap(d_a, d_b);
  }

  // Pop the front task from the queue.
  std::shared_ptr<Task> task = d_a.front();
  d_a.pop_front();

  // Run the task.
  task->run();

  // If the task is not done or canceled, push the task back to the B queue.
  if (task->alive()) {
    std::unique_lock<std::mutex> lkB(d_bMutex);
    d_b.push_back(task);
  }  
}
