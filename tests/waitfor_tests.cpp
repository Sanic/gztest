#include "gtest/gtest.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include "gztest/gztest_helpers.h"

// Use the helper methods from gztest
gztest::TestHelper th;

bool global_bool = true;
// Simple test function for condition checking
bool returnBool(bool b)
{
  return b;
}

bool toggleGlobalBool()
{
  global_bool = !global_bool;
  return global_bool;
}

TEST (TestWaitFor, returnImmediatelyWithTrue) { 
    ASSERT_TRUE (th.waitForTrue(boost::bind (returnBool, true), 2000));
}

TEST (TestWaitFor, returnAfterTimeoutWithFalse) { 
    ASSERT_FALSE(th.waitForTrue(boost::bind (returnBool, false), 4000));
}

// The condition is false in the first iteration
// In the next test, the method should return true and therefore waitForTrue should return true
// This method should wait for around 1000ms
TEST (TestWaitFor, returnBeforeTimeoutWithTrue) { 
    ASSERT_TRUE(th.waitForTrue(boost::bind (toggleGlobalBool), 4000));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
