#include "gztest/test_helpers.h"

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

bool TestHelper::positionInRange(std::vector<double>& position, double x, double y, double z, double error)
{
	return valueInRange(position[0], x, error) && // x in range
			valueInRange(position[1], y, error) && // y in range
			valueInRange(position[2], z, error); // z in range
}

bool TestHelper::valueInRange(double value, double toCompare, double error)
{
	return value - error < toCompare && value + error > toCompare;
}
