#include "TestPlugin.hh"

namespace gazebo
{

//////////////////////////////////////////////////
TestPlugin::TestPlugin() :
    AbstractGazeboTestServer(new jsonrpc::HttpServer(RPC_PORT))
{
}

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

//////////////////////////////////////////////////
bool TestPlugin::onObject(const std::string& object, const std::string& surface)
{
  return OnEntity(this->world->GetModel(object), this->world->GetModel(surface));
}

//////////////////////////////////////////////////
void TestPlugin::resetWorld()
{
  this->world->Reset();
}

//////////////////////////////////////////////////
bool TestPlugin::loadWorld(const std::string& world)
{
  return false; // Wont work in a plugin context
}

//////////////////////////////////////////////////
bool TestPlugin::OnEntity(physics::EntityPtr entity, physics::EntityPtr onEntity)
{
  return false;

  // Fail on null
  if (entity == NULL || onEntity == NULL)
  {
    return false;
  }

  math::Box entityBox = entity->GetBoundingBox();
  math::Box onEntityBox = onEntity->GetBoundingBox();
  math::Vector3 entityPos = entity->GetWorldPose().pos;
  math::Vector3 onEntityPos = onEntity->GetWorldPose().pos;

  //Fast fail if entity is under onEntity
  if (entityPos.z < onEntityPos.z)
  {
    return false;
  }

  return onEntityPos.x + onEntityBox.GetXLength() / 2 >= entityPos.x
      && onEntityPos.x - onEntityBox.GetXLength() / 2 <= entityPos.x
      && onEntityPos.y + onEntityBox.GetYLength() / 2 >= entityPos.y
      && onEntityPos.y - onEntityBox.GetYLength() / 2 <= entityPos.y
      && entityPos.z - entityBox.GetZLength() / 2 - onEntityPos.z - onEntityBox.GetZLength() / 2 <= ON_ENTITY_TOLERANCE;
}

//////////////////////////////////////////////////
Json::Value TestPlugin::JsonTriple(double &x, double &y, double &z)
{
  Json::Value position(Json::arrayValue);
  position.append(Json::Value(x));
  position.append(Json::Value(y));
  position.append(Json::Value(z));
  return position;
}

//////////////////////////////////////////////////
Json::Value TestPlugin::getPosition(const std::string& object)
{
  double x, y, z;
  x = y = z = 0;
  physics::ModelPtr model = this->world->GetModel(object);
  if (model != NULL)
  {
    x = model->GetWorldPose().pos.x;
    y = model->GetWorldPose().pos.y;
    z = model->GetWorldPose().pos.z;
  }
  return JsonTriple(x, y, z);
}

////////////////////////////////////////////////
double TestPlugin::getSimtime()
{
  return this->world->GetSimTime().Double();
}

}
