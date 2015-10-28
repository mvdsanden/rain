#ifndef __INC_RAIN_IO_IO_STATE_MONITOR_H__
#define __INC_RAIN_IO_IO_STATE_MONITOR_H__

#include "util/singleton.hh"

#include <chrono>

#include <signal.h>

namespace rain {
  namespace io {

    class IoStateChangedHandler;
    
    class IoStateMonitor
      : public util::Singleton<IoStateMonitor>
    {
    protected:

    public:

      IoStateMonitor();
      
      ~IoStateMonitor();
      
      void update(std::chrono::steady_clock::duration const &timeout);

      void registerFileDescriptor(int fd, IoStateChangedHandler *handler);

      void unregisterFileDescriptor(int fd);

    private:

      int d_epoll;

      sigset_t d_signalMask;

      size_t d_tableSize;
      
      IoStateChangedHandler **d_table;
      
    };
    
  }
}

#endif // __INC_RAIN_IO_IO_STATE_MONITOR_H__
