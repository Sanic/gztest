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
  this->updateConnection = event::Events::ConnectWorldUpdateEnd(
      boost::bind(&AbstractTestImplementation::onUpdate, this));
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

//////////////////////////////////////////////////
bool AbstractTestImplementation::monitorLinkEvents(const std::string& modelName, const std::string& jointName,
                                                   const std::string& linkName)
{
  physics::ModelPtr model = GetWorld()->GetModel(modelName);
  if (model == NULL)
  {
    return false;
  }
  physics::JointPtr joint = model->GetJoint(jointName);
  if (joint == NULL)
  {
    return false;
  }
  Watcher watcher(modelName, jointName, linkName);
  this->linkWatchEvents.insert(std::make_pair(watcher, std::vector<WatcherEvent>()));
  return true;
}

//////////////////////////////////////////////////
void AbstractTestImplementation::onUpdate()
{
  updateJoints();
}

//////////////////////////////////////////////////
void AbstractTestImplementation::updateJoints()
{
  for (WatcherEventMap::iterator iter = this->linkWatchEvents.begin(); iter != this->linkWatchEvents.end(); ++iter)
  {
    physics::ModelPtr model = GetWorld()->GetModel(iter->first.modelName);
    physics::JointPtr joint = model->GetJoint(iter->first.jointName);
    physics::Link_V childLinks = joint->GetChild()->GetChildJointsLinks();
    physics::LinkPtr link;
    for (physics::Link_V::iterator jointLinks = childLinks.begin(); jointLinks != childLinks.end(); ++jointLinks)
    {
      if ((*jointLinks)->GetName() == iter->first.linkName)
      {
        link = (*jointLinks);
        break;
      }
    }
    bool attached = link != NULL;
    std::vector<WatcherEvent>* watcherList = &iter->second;
    if (watcherList->size() == 0 || watcherList->back().get<1>() != attached
        || (attached && watcherList->back().get<2>() != link->GetModel()->GetName()))
    {
      watcherList->push_back(WatcherEvent(getSimtime(), attached, attached ? link->GetModel()->GetName() : ""));
    }
  }

}

//////////////////////////////////////////////////
Json::Value AbstractTestImplementation::getLinkEventHistory(const std::string& modelName, const std::string& jointName,
                                                            const std::string& linkName)
{
  Json::Value all(Json::arrayValue);
  WatcherEventMap::const_iterator history = this->linkWatchEvents.find(Watcher(modelName, jointName, linkName));
  if (history != this->linkWatchEvents.end())
  {
    for (std::vector<WatcherEvent>::const_iterator historyElement = history->second.begin();
        historyElement != history->second.end(); ++historyElement)
    {
      Json::Value position(Json::arrayValue);
      position.append((*historyElement).head);
      position.append((*historyElement).tail.head);
      position.append((*historyElement).tail.tail.head);
      all.append(position);
    }
  }
  return all;
}

} /* namespace gazebo */
