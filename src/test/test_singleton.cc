#include <iostream>

#include "util/singleton.hh"


class Test
  : public rain::util::Singleton<Test>
{
public:

  Test()
  {
    std::cout << "Singleton initialized.\n";
  }

  ~Test()
  {
    std::cout << "Singleton destroyed.\n";
  }
  
  void test()
  {
    std::cout << "It works! :).\n";
  }
  
};


int main()
{
  Test &test = Test::getInstance();

  test.test();
  
  return 0;
}
