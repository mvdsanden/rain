#include <iostream>

#include <thread>

#include <unistd.h>

#include "core/task.hh"
#include "core/taskcompletionhandler.hh"

rain::core::FunTask t([](){
    std::cout << "Task run!\n";
    sleep(1);
    std::cout << "Task done!\n";
    return true;
  });

int main()
{
  rain::core::FunTaskCompletionHandler completed([](rain::core::Task &task, rain::core::TaskCompletionHandler::TaskState state)
						 {
						   std::cout << "Task completed: " << state << ".\n";
						 });
  
  t.setCompletionHandler(&completed);
  
  std::thread th([](){ t.run(); });

  std::cout << "Waiting for t...\n";
  t.wait();
  std::cout << "Done waiting...\n";

  th.join();
  
  return 0;
}
