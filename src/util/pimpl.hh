#ifndef __INC_RAIN_PIMPL_HH_
#define __INC_RAIN_PIMPL_HH_

namespace rain {

  /**
   *  Pointer to implementation pattern.
   */
  struct pimpl {

    /**
     *  Base class for all implementation classes.
     */
    class implementation {
    public:

      /**
       *  Clones the implementation by using the copy constructor.
       */
      virtual implementation *clone() const
      {
	return new implementation(*this);
      }
    
    };
  
    struct pointer_semantics;
    struct value_semantics;

    class Base;
  
    template <class T>
    class Object;
  
  };

  /**
   *  The base for all pimpl classes.
   */
  class pimpl::Base {
  public:

    typedef pimpl::implementation implementation;
  
    implementation *d_impl;

    Base(implementation *impl)
      : d_impl(impl)
    {
    }
  
  };

  template <class CopyPolicy>
  class pimpl::Object : public pimpl::Base {
  protected:
  
    Object(implementation *impl)
      : Base(impl)
    {
    }

    Object(Object const &other)
      : Base(CopyPolicy::copy(other.d_impl))
    {
    }

  };

  /**
   *  Pointer semantics copy policy.
   *
   *  Causes only the pointer to be copied.
   */
  struct pimpl::pointer_semantics {

    static implementation *copy(implementation *impl)
    {
      return impl;
    }
  
  };
  
  /**
   *  Value semantics copy policy.
   *
   *  Causes all data members to be copied.
   *
   *  Note: the implementation structure should implement
   *        the clone() method.
   */
  struct pimpl::value_semantics {

    static implementation *copy(implementation *impl)
    {
      return impl->clone();
    }
  
  };

  template <class T>
  typename T::implementation &operator*(T &obj)
  {
    static_assert(std::is_base_of<pimpl::Base, T>::value, "object should be derived from pimpl::Base");
    return *static_cast<typename T::implementation*>(obj.d_impl);
  }

  template <class T>
  typename T::implementation const &operator*(T const &obj)
  {
    static_assert(std::is_base_of<pimpl::Base, T>::value, "object should be derived from pimpl::Base");
    return *static_cast<typename T::implementation const*>(obj.d_impl);
  }
  
}

#endif // __INC_RAIN_PIMPL_HH_

