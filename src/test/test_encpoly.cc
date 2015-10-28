class Foo {
protected:
  
  struct implementation;
  implementation *d_impl;
  
  template <class... Args>
  Foo(implementation *impl, Args... args);
  
public:

  Foo(int a);

};


class Bar : public Foo {
protected:
  
  struct implementation;

public:

  Bar(int a, int b);

};


struct Foo::implementation {

  int a;

};


struct Bar::implementation : public Foo::implementation {

  int b;

};

template <class... Args>
Foo::Foo(implementation *impl, Args... args)
  : d_impl(impl), Foo(args)
{
  
}


Bar(int a, int b);

int main()
{
  Bar b;
  
  return 0;
}
