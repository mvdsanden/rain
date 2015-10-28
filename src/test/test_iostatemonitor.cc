#include <iostream>

#include "io/iostatemonitor.hh"
#include "io/iostatechangedhandler.hh"

#include "exceptions/errnoexception.hh"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

class TestHandler
  : public rain::io::IoStateChangedHandler
{
public:

  virtual void ioStateChanged(int fd, uint32_t stateMask)
  {
    std::cout << "ioStateChanged(" << fd << ", " << stateMask << ").\n";
  }
  
};

int main()
{
  TestHandler handler;

  auto &monitor = rain::io::IoStateMonitor::getInstance();

  int sfds[2];

  // Create a non-blocking stream socket pair.
  int ret = socketpair(AF_UNIX,
		       SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,
		       0,
		       sfds);

  if (ret == -1) {
    throw rain::exceptions::ErrnoException(errno);
  }

  int pfds[2];

  ret = pipe2(pfds, O_CLOEXEC | O_NONBLOCK);

  if (ret == -1) {
    throw rain::exceptions::ErrnoException(errno);
  }
  
  monitor.registerFileDescriptor(sfds[1], &handler);
  monitor.registerFileDescriptor(pfds[0], &handler);
  monitor.registerFileDescriptor(pfds[1], &handler);
  //  monitor.registerFileDescriptor(1, &handler);
  //  monitor.registerFileDescriptor(2, &handler);

  write(sfds[0], "abcd", 5);
  write(pfds[1], "abcd", 5);
  
  while (true) {

    monitor.update(std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::seconds(5)));

  }
  
  return 0;
}
