/*
 * TestWrapper.hh
 *
 *  Created on: Jul 14, 2014
 *      Author: moritz
 */

#ifndef TESTWRAPPER_HH_
#define TESTWRAPPER_HH_
#include <sys/wait.h>
#include "gazebo/gazebo.hh"
#include "gazebo/physics/physics.hh"
#include "abstractgazebotestserver.h"

namespace gztest
{

// Default RPC port
#define RPC_PORT 8080

// If the bounding box of an object is less than this value (in meters) over the other object, it is considered to be "on" the other object.
#define ON_ENTITY_TOLERANCE 0.01


class TestWrapper : public AbstractGazeboTestServer
{
public:
	TestWrapper();
    bool onObject(const std::string& object, const std::string& surface);
    Json::Value getPosition(const std::string& object);
    double getSimtime();
    void resetWorld();
    bool loadWorld(const std::string& world);
    void startGazebo(int _argc, char **_argv);
    void startGazeboGUI(char **_argv);
    void stopGazeboGUI();
private:
    bool OnEntity(gazebo::physics::EntityPtr entity, gazebo::physics::EntityPtr onEntity);
    void killGazeboGUI();
    Json::Value JsonTriple(double &x, double &y, double &z);
    gazebo::physics::WorldPtr replaceWorld();
	void bootWorld(const gazebo::physics::WorldPtr& new_world, const sdf::ElementPtr& worldElem);

    gazebo::Server *server;
    gazebo::physics::WorldPtr world;
    pid_t gzclientPID;
    bool terminateGZClient;
};

} /* namespace gazebo */

#endif /* TESTWRAPPER_HH_ */
