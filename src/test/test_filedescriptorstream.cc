#include "exceptions/errnoexception.hh"
#include "io/filedescriptorstream.hh"
#include "io/inputhandler.hh"
#include "io/outputhandler.hh"
#include "io/iostatemonitor.hh"
#include "io/sourceinputsession.hh"

#include <iostream>
#include <thread>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

class TestInputHandler
  : public rain::io::InputHandler
{
public:

  virtual void onInputClosed()
  {
    std::cout << "InputHandler: input closed.\n";
  }

  virtual void onInputReadable()
  {
    std::cout << "InputHandler: input readable.\n";

    auto session = std::make_shared<rain::io::SourceInputSession>(d_source);

    if (!session->isValid()) {
      std::cout << "Session invalid!\n";
    }

    auto fun = [session]()
      {
	char buffer[16];

	while (session->isValid()) {
	  size_t len = session->read(buffer, 16);
	  std::cout << "Read " << len << " bytes.\n";
	}
      };

    session.reset();

    std::thread t(fun);
    t.detach();
  }

  virtual void connectSource(std::weak_ptr<rain::io::Source> const &source)
  {
    d_source = source;
  }

private:

  std::weak_ptr<rain::io::Source> d_source;
  //  std::shared_ptr<rain::io::SourceInputSession> d_session;
  
};

class TestOutputHandler
  : public rain::io::OutputHandler
{
public:

  virtual void onOutputClosed()
  {
    std::cout << "OutputHandler: output closed.\n";
  }

  virtual void onOutputWritable()
  {
    std::cout << "OutputHandler: output writable.\n";
  }

  virtual void connectSink(std::weak_ptr<rain::io::Sink> const &sink)
  {
  }
  
};

int main()
{
  auto ihandler = std::make_shared<TestInputHandler>();
  auto ohandler = std::make_shared<TestOutputHandler>();
  
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

  std::shared_ptr<rain::io::FileDescriptorStream> ss0 = std::make_shared<rain::io::FileDescriptorStream>(sfds[0]);
  std::shared_ptr<rain::io::FileDescriptorStream> ss1 = std::make_shared<rain::io::FileDescriptorStream>(sfds[1]);

  ss0->connectInputHandler(ihandler);
  ihandler->connectSource(ss0);
  
  ss0->connectOutputHandler(ohandler);
  ohandler->connectSink(ss0);


  auto fun = [&]()
    {
      while (true) {
	int res = write(sfds[1], "abcd", 5);

	std::cout << "Written " << res << " bytes tp sfds[1].\n";
      
	sleep(1);
      }
    };
  
  std::thread t(fun);
  
  auto &monitor = rain::io::IoStateMonitor::getInstance();

  while (true) {

    monitor.update(std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::seconds(5)));

    //    write(sfds[1], "abcd", 5);
    
  }

  t.join();
  
  return 0;
}
