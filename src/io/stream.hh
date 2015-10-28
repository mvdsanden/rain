#ifndef __INC_RAIN_IO_STREAM_HH__
#define __INC_RAIN_IO_STREAM_HH__

#include "util/visibility.hh"

#include "source.hh"
#include "sink.hh"

#include <memory>

namespace rain {
  namespace io {

    class CLASS_PUBLIC Stream
      : public Sink,
	public Source
    {
    public:

    };
    
  }
}

#endif // __INC_RAIN_IO_STREAM_HH__
