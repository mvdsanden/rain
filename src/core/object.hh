#ifndef __INC_RAIN_OBJECT_HH__
#define __INC_RAIN_OBJECT_HH__

#include <memory>

namespace rain {

  
  
  template <class T>
  struct Object {

    struct implementation;
    
    template <class, class> class pimpl;
    class shared_pimpl;
    //    template <class, template <class> > unique_pimpl;
    
    typedef pimpl<T, shared_pimpl> pointer_symantics;
    //    typedef Pimpl<T, unique_pimpl> value_symantics;

  };

  template <class T>
  struct Object<T>::implementation {
  };
  
  template <class T>
  template <class traits>
  class Object<T>::pimpl<T, traits> {    

    typename traits::impl_ptr d_impl;

  protected:

    typedef pimpl<T, traits> base;
    
  public:

    template <class>
    class Inherit : public T {
    public:

      struct implementation;
      
    };
    
    template <class... Args>
    pimpl(Args... args)
      : d_impl(new Object<T>::implementation(args...))
    {

    }
    
  protected:

    Object<T>::implementation &operator*() { return *d_impl.get(); }
    Object<T>::implementation const &operator*() const { return *d_impl.get(); }
    Object<T>::implementation *operator->() { return d_impl.get(); }
    Object<T>::implementation const *operator->() const { return d_impl.get(); }

  };
  
  template <class T>
  struct Object<T>::shared_pimpl {

    typedef std::shared_ptr<Object<T>::implementation> impl_ptr;
    
  };

}

#endif // __INC_RAIN_OBJECT_HH__
