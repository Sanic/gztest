#ifndef _TEST_PLUGIN_HH_
#define _TEST_PLUGIN_HH_

#include "AbstractTestImplementation.hh"

namespace gazebo
{

#ifdef USE_SYSTEM_PLUGIN
class TestPlugin : public SystemPlugin, public gztest::AbstractTestImplementation
#else
class TestPlugin : public WorldPlugin, public gztest::AbstractTestImplementation
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
  bool loadWorld(const std::string& world);
private:
  physics::WorldPtr GetWorld();

  physics::WorldPtr world;
};

#ifdef USE_SYSTEM_PLUGIN
GZ_REGISTER_SYSTEM_PLUGIN(TestPlugin)
#else
GZ_REGISTER_WORLD_PLUGIN(TestPlugin)
#endif
}
#endif
