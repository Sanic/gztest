#include "gztest/gztest_helpers.h"

using namespace gztest;

void TestHelper::sleep(int ms)
{
  boost::this_thread::sleep(boost::posix_time::milliseconds(ms));
}

bool TestHelper::waitForTrue(boost::function<bool()> x, int msec)
{
  boost::posix_time::ptime start_time = boost::posix_time::microsec_clock::local_time();
  boost::posix_time::ptime current_time = boost::posix_time::microsec_clock::local_time();
  while( (current_time - start_time).total_milliseconds() < msec)
  {
    if(x())
      return true;
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
    current_time = boost::posix_time::microsec_clock::local_time();
  }
  return false;
}
