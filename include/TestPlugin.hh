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

#ifdef USE_SYSTEM_PLUGIN
  class TestPlugin : public SystemPlugin, public AbstractGazeboTestServer
#else
  class TestPlugin : public WorldPlugin, public AbstractGazeboTestServer
#endif
  {
    public:
      TestPlugin();
#ifdef USE_SYSTEM_PLUGIN
      void Load(int _argc, char **_argv);
#else
      void Load(physics::WorldPtr _world, sdf::ElementPtr _sdf);
#endif
      void Init();
      bool onObject(const std::string& object, const std::string& surface);
      void resetWorld();
      bool loadWorld(const std::string& world);
      Json::Value getPosition(const std::string& object);
      double getSimtime();
    private:
      Json::Value JsonTriple(double &x, double &y, double &z);
      bool OnEntity(physics::EntityPtr entity, physics::EntityPtr onEntity);
      physics::WorldPtr world;
  };

#ifdef USE_SYSTEM_PLUGIN
  GZ_REGISTER_SYSTEM_PLUGIN(TestPlugin)
#else
  GZ_REGISTER_WORLD_PLUGIN(TestPlugin)
#endif
}
#endif
