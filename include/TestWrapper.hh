/*
 * TestWrapper.hh
 *
 *  Created on: Jul 14, 2014
 *      Author: moritz
 */

#ifndef TESTWRAPPER_HH_
#define TESTWRAPPER_HH_
#include <sys/wait.h>
#include "AbstractTestImplementation.hh"

namespace gztest
{

class TestWrapper : public AbstractTestImplementation
{
public:
  TestWrapper();
  bool loadWorld(const std::string& world);
  void startGazebo(int _argc, char **_argv);
  void startGazeboGUI(char **_argv);
  void stopGazeboGUI();
private:
  gazebo::physics::WorldPtr GetWorld();
  void killGazeboGUI();
  gazebo::physics::WorldPtr replaceWorld();
  void bootWorld(const gazebo::physics::WorldPtr& new_world, const sdf::ElementPtr& worldElem);

  gazebo::Server *server;
  gazebo::physics::WorldPtr world;
  pid_t gzclientPID;
  bool terminateGZClient;
};

} /* namespace gazebo */

#endif /* TESTWRAPPER_HH_ */
