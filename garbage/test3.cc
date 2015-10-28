#include <iostream>

#include "../src/util/pimpl.hh"

using namespace rain;

class Foo : public pimpl::Object<pimpl::value_semantics> {
public:

  class implementation;
  
protected:

  Foo(implementation *impl);
  
public:

  Foo(int a);
  
  int test();

  void setA(int a);
  
};

class Bar : public Foo {
public:

  class implementation;

protected:

  Bar(implementation *impl);
  
public:

  Bar(int a, int b); 

  int test();
  
};

class Foo::implementation : public Base::implementation {
protected:
  int d_a;
public:

  implementation(int a)
    : d_a(a)
  {
  }

  virtual implementation *clone() const
  {
    return new implementation(*this);
  }
  
  int test();

  void setA(int a);
};

class Bar::implementation : public Foo::implementation {
protected:
  int d_b;
public:

  implementation(int a, int b)
    : Foo::implementation(a), d_b(b)
  {
  }

  virtual implementation *clone() const
  {
    return new implementation(*this);
  }
  
  int test();
};

Foo::Foo(implementation *impl)
  : Object(impl)
{
}

Foo::Foo(int a)
  : Foo(new implementation(a))
{
}

int Foo::test()
{
  return (**this).test();
}

int Foo::implementation::test()
{
  return d_a;
}

void Foo::implementation::setA(int a)
{
  d_a = a;
}

void Foo::setA(int a)
{
  (**this).setA(a);
}

Bar::Bar(implementation *impl)
  : Foo(impl)
{
}

Bar::Bar(int a, int b)
  : Bar(new implementation(a, b))
{
}

int Bar::implementation::test()
{
  return d_b + d_a;
}

int Bar::test()
{
  return (**this).test();
}

int main()
{
  Bar b(1, 2);
  Foo f(2);

  Bar c(b);
  
  std::cout << b.test() << ".\n";

  std::cout << f.test() << ".\n";

  std::cout << c.test() << ".\n";

  c.setA(10);

  std::cout << b.test() << ".\n";

  std::cout << f.test() << ".\n";

  std::cout << c.test() << ".\n";
  
  return 0;
}
