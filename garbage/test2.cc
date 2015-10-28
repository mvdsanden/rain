#include <iostream>

class Base {
public:

  struct implementation {};

  //private:
  
  implementation *d_impl;
  
protected:

  Base(implementation *impl)
    : d_impl(impl)
  {
  }
    
  template <class T>
  typename T::implementation &impl(T *t)
  {
    return *static_cast<typename T::implementation *>(d_impl);
  }

  template <class T>
  typename T::implementation const &impl(T const *t) const
  {
    return *static_cast<typename T::implementation const *>(d_impl);
  }

public:

};

template <class T>
typename T::implementation &operator*(T &obj)
{
  return *static_cast<typename T::implementation*>(obj.d_impl);
}

template <class T>
typename T::implementation const &operator*(T const &obj)
{
  return *static_cast<typename T::implementation const*>(obj.d_impl);
}

class Foo : public Base {
public:

  struct implementation;
  
protected:

  Foo(implementation *impl);
  
  Foo(implementation *impl, int a);
  
public:

  Foo(int a);
  
  int test();
  
};

class Bar : public Foo {
public:

  struct implementation;

protected:

  Bar(implementation *impl);
  
  Bar(implementation *impl, int a, int b);
  
public:

  Bar(int a, int b); 

  int test();
  
};

struct Foo::implementation : public Base::implementation {
  int a;
};

struct Bar::implementation : public Foo::implementation {
  int b;
};

Foo::Foo(implementation *impl)
  : Base(impl)
{
}

Foo::Foo(implementation *impl, int a)
  : Base(impl)
{
  impl->a = a;
}

Foo::Foo(int a)
  : Foo(new implementation, a)
{
}

int Foo::test()
{
  return (**this).a;
}

Bar::Bar(implementation *impl)
  : Foo(impl)
{
}

Bar::Bar(implementation *impl, int a, int b)
  : Foo(impl, a)
{
  (**this).b = b;
}

Bar::Bar(int a, int b)
  : Bar(new implementation, a, b)
{
}

int Bar::test()
{
  auto &i = **this;
  i.b = 12;
  return i.b + i.a;
}


int main()
{
  Bar b(1, 2);
  Foo f(2);

  std::cout << b.test() << ".\n";

  std::cout << f.test() << ".\n";
  
  return 0;
}
