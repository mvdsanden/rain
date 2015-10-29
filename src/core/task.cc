#include "task.hh"

#include "taskcompletionhandler.hh"

using namespace rain;
using namespace core;

Task::Task()
  : d_state(SCHEDULED)
{
}

Task::~Task()
{
  //std::unique_lock<std::mutex> lk(d_mutex);
  //if (d_state < DONE) {
  //  throw std::runtime_error("task deleted before it was finished");
  //}
}
      
void Task::run()
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

void Task::reset()
{
  std::unique_lock<std::mutex> lk(d_mutex);

  if (d_state <= RUNNING) {
    return;
  }

  d_state = SCHEDULED;
}
      
void Task::cancel()
{
  std::unique_lock<std::mutex> lk(d_mutex);

  while (d_state == RUNNING) {
    d_cond.wait(lk);
  }

  d_state = CANCELED;	
  d_cond.notify_all();

  if (d_completionHandler) {
    d_completionHandler->onTaskCompleted(*this, TaskCompletionHandler::TASK_CANCELED);
  }
}
      
bool Task::alive() const
{
  return d_state <= RUNNING;
}

bool Task::canceled() const
{
  return d_state == CANCELED;
}

void Task::wait()
{
  std::unique_lock<std::mutex> lk(d_mutex);
  while (d_state < DONE) {
    d_cond.wait(lk);
  }
}

void Task::done()
{
  std::unique_lock<std::mutex> lk(d_mutex);
  d_state = DONE;
  d_cond.notify_all();

  if (d_completionHandler) {
    d_completionHandler->onTaskCompleted(*this, TaskCompletionHandler::TASK_DONE);
  }
}

void Task::setCompletionHandler(TaskCompletionHandler *handler)
{
  d_completionHandler = handler;
}
