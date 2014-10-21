#ifndef _ABSTRACT_TEST_IMPLEMENTATION_HH_
#define _ABSTRACT_TEST_IMPLEMENTATION_HH_

#include "gazebo/gazebo.hh"
#include "gazebo/physics/physics.hh"
#include "Watchers.hh"
#include "abstractgazebotestserver.h"

namespace gztest
{

// Default RPC port
#define RPC_PORT 8080

// If the bounding box of an object is less than this value (in meters) over the other object, it is considered to be "on" the other object.
#define ON_ENTITY_TOLERANCE 0.01

class AbstractTestImplementation : public AbstractGazeboTestServer
{
public:
  AbstractTestImplementation(jsonrpc::AbstractServerConnector* conn);
  void resetWorld();
  bool onObject(const std::string& object, const std::string& surface);
  Json::Value getPosition(const std::string& object);
  double getSimtime();
  bool monitorLinkEvents(const std::string& modelName, const std::string& jointName, const std::string& linkName);
  Json::Value getLinkEventHistory(const std::string& modelName, const std::string& jointName,
                                  const std::string& linkName);
protected:
  Json::Value JsonTriple(double &x, double &y, double &z);
  bool OnEntity(gazebo::physics::EntityPtr entity, gazebo::physics::EntityPtr onEntity);
  virtual gazebo::physics::WorldPtr GetWorld() = 0;
private:
  gazebo::event::ConnectionPtr updateConnection;
  WatcherEventMap linkWatchEvents;
  void onUpdate();
  void updateJoints();
};

}
#endif
