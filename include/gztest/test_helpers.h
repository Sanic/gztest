#ifndef GZTEST_HELPERS_H
#define GZTEST_HELPERS_H

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace gztest
{
  class TestHelper
  {
    // use boost::this_thread::sleep to sleep ms milliseconds
    public: void sleep(int ms);

    // Takes a function pointer and a timeout
    // The given function will be called every 100 msec, until it returns true
    // If the function returns true, waitForTrue returns true.
    // Otherwise, the method will return false after msec millseconds
    public: bool waitForTrue(boost::function<bool()> x, int msec);
  };
}
#endif
