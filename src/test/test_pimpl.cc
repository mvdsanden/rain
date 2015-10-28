#include "../core/object.hh"

#include <iostream>

using namespace rain;

class Test : public Object<Test>::pointer_symantics {
public:

  Test(int value);
  
  int value() const;
  
};

class Foo : public Test::Inherit<Foo> {
public:

  int bla() const;
  
};

namespace rain {
  
  template <> struct Object<Test>::implementation {

    implementation(int v)
      : value(v)
    {
    }
    
    int value;
  
  };

  template <> struct Test::Inherit<Foo>::implementation : public Object<Test>::implementation {

    implementation(int b)
      : bla(b)
    {
    }
    
    int bla;
  
  };

}

Test::Test(int value)
  : base(value)
{
}

int Test::value() const
{
  return (*this)->value;
}

int main()
{
  Test t(11);

  std::cout << t.value() << ".\n";
  
  return 0;
}
