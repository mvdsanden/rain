#ifndef __INC_RAIN_UTIL_SINGLETON_HH__
#define __INC_RAIN_UTIL_SINGLETON_HH__

namespace rain {
  namespace util {

    /**
     *  TODO: implement symentics for leaky pointer singleton.
     */
    template <class T>
    class Singleton {

      Singleton(Singleton const &);
      Singleton &operator=(Singleton const &);
      
    protected:

      Singleton()
      {
      }
      
    public:

      ~Singleton()
      {
      }
      
      static T &getInstance()
      {
	static T s_instance;
	return s_instance;
      }
      
    };
    
  }
}

#endif // __INC_RAIN_UTIL_SINGLETON_HH__
