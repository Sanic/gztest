#include "TestPlugin.hh"

namespace gazebo
{

//////////////////////////////////////////////////
TestPlugin::TestPlugin() :
    AbstractTestImplementation(new jsonrpc::HttpServer(RPC_PORT))
{
  gzmsg << "TestPlugin started" << std::endl;
}

/**
 * START PLUGIN ENTRYPOINT METHODS
 */

#ifdef USE_SYSTEM_PLUGIN
//////////////////////////////////////////////////
void TestPlugin::Load(int _argc, char **_argv)
{
  // Do nothing on server load
}
#else
//////////////////////////////////////////////////
void TestPlugin::Load(physics::WorldPtr _world, sdf::ElementPtr _sdf)
{
  Init();
}
#endif

//////////////////////////////////////////////////
void TestPlugin::Init()
{
  this->world = physics::get_world();
  StartListening();
}

/**
 * START ABSTRACT METHOD IMPLEMENTATIONS
 */

//////////////////////////////////////////////////
bool TestPlugin::loadWorld(const std::string& world)
{
  return false; // Wont work in a plugin context
}

//////////////////////////////////////////////////
physics::WorldPtr TestPlugin::GetWorld()
{
  return this->world;
}

}
