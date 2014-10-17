/*
 * AbstractTestImplementation.cpp
 *
 *  Created on: Oct 17, 2014
 *      Author: moritz
 */

#include "AbstractTestImplementation.hh"

using namespace gazebo;

namespace gztest
{

//////////////////////////////////////////////////
AbstractTestImplementation::AbstractTestImplementation(jsonrpc::AbstractServerConnector* conn) :
    AbstractGazeboTestServer(conn)
{
}

//////////////////////////////////////////////////
bool AbstractTestImplementation::onObject(const std::string& object, const std::string& surface)
{
  return OnEntity(GetWorld()->GetModel(object), GetWorld()->GetModel(surface));
}

//////////////////////////////////////////////////
void AbstractTestImplementation::resetWorld()
{
  GetWorld()->Reset();
}

//////////////////////////////////////////////////
bool AbstractTestImplementation::OnEntity(physics::EntityPtr entity, physics::EntityPtr onEntity)
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
Json::Value AbstractTestImplementation::JsonTriple(double &x, double &y, double &z)
{
  Json::Value position(Json::arrayValue);
  position.append(Json::Value(x));
  position.append(Json::Value(y));
  position.append(Json::Value(z));
  return position;
}

//////////////////////////////////////////////////
Json::Value AbstractTestImplementation::getPosition(const std::string& object)
{
  double x, y, z;
  x = y = z = 0;
  physics::ModelPtr model = GetWorld()->GetModel(object);
  if (model != NULL)
  {
    x = model->GetWorldPose().pos.x;
    y = model->GetWorldPose().pos.y;
    z = model->GetWorldPose().pos.z;
  }
  return JsonTriple(x, y, z);
}

////////////////////////////////////////////////
double AbstractTestImplementation::getSimtime()
{
  return GetWorld()->GetSimTime().Double();
}

} /* namespace gazebo */
