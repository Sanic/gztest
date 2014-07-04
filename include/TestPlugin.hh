#ifndef _TEST_PLUGIN_HH_
#define _TEST_PLUGIN_HH_

#include "gazebo/gazebo.hh"
#include "gazebo/physics/physics.hh"
#include "abstractgazebotestserver.h"

namespace gazebo
{

  // Default RPC port
  #define RPC_PORT 8080

  // If the bounding box of an object is less than this value (in meters) over the other object, it is considered to be "on" the other object.
  #define ON_ENTITY_TOLERANCE 0.01

  class TestPlugin : public WorldPlugin, public AbstractGazeboTestServer
  {
    public:
	  TestPlugin();
      void Load(physics::WorldPtr _world, sdf::ElementPtr _sdf);
      void OnUpdate(const common::UpdateInfo & /*_info*/);
      bool onObject(const std::string& object, const std::string& surface);
    private:
      bool OnEntity(physics::EntityPtr entity, physics::EntityPtr onEntity);
      event::ConnectionPtr updateConnection;
      physics::WorldPtr world;
  };

  GZ_REGISTER_WORLD_PLUGIN(TestPlugin)
}
#endif
