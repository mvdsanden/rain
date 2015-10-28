#include "iostatemonitor.hh"

#include "io.hh"
#include "iostatechangedhandler.hh"
#include "exceptions/errnoexception.hh"

#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/resource.h>

#include <unistd.h>
#include <string.h>

#include <iostream>

#include <cassert>

using namespace rain;
using namespace io;
using namespace exceptions;

IoStateMonitor::IoStateMonitor()
{
  // Initialize handler table for every possible file descriptor.
  d_tableSize = Io::getInstance().getFileDescriptorLimit();
  d_table = new IoStateChangedHandler * [ d_tableSize ];
  memset(d_table, 0, sizeof(IoStateChangedHandler) * d_tableSize);
  
  // Create the epoll handle.
  d_epoll = epoll_create1(EPOLL_CLOEXEC);
    
  if (d_epoll == -1) {
    throw ErrnoException(errno);
  }

  // Setup the polling signal mask.
  sigemptyset(&d_signalMask);
  sigaddset(&d_signalMask, SIGPIPE);
}

IoStateMonitor::~IoStateMonitor()
{
  close(d_epoll);
}

uint32_t _rain_io_convert_epoll_events(uint32_t events)
{
  return
    ((events & EPOLLIN)?READ_BIT:0) |
    ((events & EPOLLPRI)?PRIORITY_BIT:0) |
    ((events & EPOLLOUT)?WRITE_BIT:0) |
    ((events & EPOLLERR)?ERROR_BIT:0) |
    ((events & EPOLLHUP)?HANGUP_BIT:0) |
    ((events & EPOLLRDHUP)?READ_HANGUP_BIT:0);    
}

void IoStateMonitor::update(std::chrono::steady_clock::duration const &timeout)
{
  int dt = std::chrono::duration_cast<std::chrono::milliseconds>(timeout).count();
  
  epoll_event pollEvents[128] = {0};

  std::cout << "Waiting for " << dt << "ms.\n";
  
  // Poll for events.
  int ret = epoll_pwait(d_epoll,
			pollEvents,
			128,
			dt,
			&d_signalMask);

  std::cout << "epoll_wait->" << ret << ".\n";
  
  if (ret == -1) {
    if (errno != EINTR) {
      throw ErrnoException(errno);
    } else {
      ret = 0;
    }
  }

  for (int i = 0; i < ret; ++i) {

    assert(pollEvents[i].data.fd >= 0 && pollEvents[i].data.fd < d_tableSize);
    
    IoStateChangedHandler *handler = d_table[pollEvents[i].data.fd];

    if (handler != NULL) {    
      handler->ioStateChanged(pollEvents[i].data.fd,
			      _rain_io_convert_epoll_events(pollEvents[i].events));
    }
    
  }
  
}

void IoStateMonitor::registerFileDescriptor(int fd, IoStateChangedHandler *handler)
{
  if (fd < 0 || fd > d_tableSize) {
    throw std::runtime_error("invalid file descriptor specified");
  }
  
  // Check if the table is empty.
  if (d_table[fd] != NULL) {
    throw std::runtime_error("there already was a handler registered for this file descriptor");
  }

  // Register the handler.
  d_table[fd] = handler;

  // Setup the epoll event structure.
  epoll_event event;
  memset(&event, 0, sizeof(event));
  event.data.fd = fd;
  event.events = EPOLLIN | EPOLLOUT | EPOLLET;

  // Add the fd to the polling queue.
  int ret = epoll_ctl(d_epoll,
		      EPOLL_CTL_ADD,
		      fd,
		      &event);

  if (ret == -1) {
    throw ErrnoException(errno);
  }
}

void IoStateMonitor::unregisterFileDescriptor(int fd)
{
  if (fd < 0 || fd > d_tableSize) {
    throw std::runtime_error("invalid file descriptor specified");
  }

  int ret = epoll_ctl(d_epoll,
		      EPOLL_CTL_DEL,
		      fd,
		      NULL);

  if (ret == -1) {
    throw ErrnoException(errno);
  }

  d_table[fd] = NULL;
}
