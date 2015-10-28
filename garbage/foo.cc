#include "foo.ih"

Foo::implementation::implementation(int value)
  : value(value)
{
}

Foo::Foo(int value)
  : base(value)
{
}

Foo::~Foo()
{
}
    
int Foo::value() const
{
  return (*this)->value;
}
