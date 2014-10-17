#ifndef GZTEST_HELPERS_H
#define GZTEST_HELPERS_H

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace gztest
{
class TestHelper
{
public:
  // use boost::this_thread::sleep to sleep ms milliseconds
  void sleep(int ms);

public:
  // Takes a function pointer and a timeout
  // The given function will be called every 100 msec, until it returns true
  // If the function returns true, waitForTrue returns true.
  // Otherwise, the method will return false after msec millseconds
  bool waitForTrue(boost::function<bool()> x, int msec);

public:
  bool positionInRange(std::vector<double>& position, double x, double y, double z, double error);
public:
  bool valueInRange(double value, double toCompare, double error);
};
}
#endif
