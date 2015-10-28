#include <iostream>

#include <thread>

#include <unistd.h>

#include "core/task.hh"
#include "core/taskrunner.hh"

std::function<bool ()> fun0 = [](){
  std::cout << "Task0\n";
  return false;
};

std::function<bool ()> fun1 = [](){
  std::cout << "Task1\n";
  return false;
};

std::function<bool ()> fun2 = [](){
  std::cout << "Task2\n";
  return false;
};

int main()
{
  rain::core::DefaultTaskRunner runner;

  auto t0 = std::make_shared<rain::core::FunTask>(fun0);
  
  runner.submitTask(t0);
  runner.submitTask(std::make_shared<rain::core::FunTask>(fun1));
  runner.submitTask(std::make_shared<rain::core::FunTask>(fun2));

  size_t count = 0;
  
  while (count < 10000) {

    std::cout << "next().\n";
    
    runner.next(std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::seconds(5)));

    if (++count == 1000) {
      t0->cancel();
    }
    
  }
  
  return 0;
}
