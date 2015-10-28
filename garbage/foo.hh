#ifndef __INC_RAIN_FOO_HH__
#define __INC_RAIN_FOO_HH__

namespace rain {

  class Foo : public Object<Foo>::pointer_symantics {
  public:

    Foo(int value);

    ~Foo();
    
    int value() const;
    
  };

}

#endif // __INC_RAIN_FOO_HH__
