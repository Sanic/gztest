#include "gztest/test_client.h"

using namespace gztest;

//////////////////////////////////////////////////////////
TestClient::TestClient(std::string address)
{
  this->client = new GazeboTestClient(new jsonrpc::HttpClient(address));
}

//////////////////////////////////////////////////////////
TestClient::~TestClient()
{
  delete this->client;
}

//////////////////////////////////////////////////////////
bool TestClient::OnObject(const std::string& object, const std::string& surface)
{
  return this->client->onObject(object, surface);
}

//////////////////////////////////////////////////////////
void TestClient::ResetWorld()
{
  this->client->resetWorld();
}

//////////////////////////////////////////////////////////
bool TestClient::LoadWorld(const std::string& world)
{
  return this->client->loadWorld(world);
}

//////////////////////////////////////////////////////////
std::vector<double> TestClient::GetPosition(const std::string& object)
{
  Json::Value position = this->client->getPosition(object);
  std::vector<double> out(3);
  out[0] = position[0u].asDouble();
  out[1] = position[1u].asDouble();
  out[2] = position[2u].asDouble();
  return out;
}

//////////////////////////////////////////////////////////
double TestClient::GetSimtime()
{
  return this->client->getSimtime();
}

//////////////////////////////////////////////////////////
std::vector<WatcherEvent> TestClient::GetLinkEventHistory(const std::string& modelName, const std::string& jointName, const std::string& linkName)
{
  std::vector<WatcherEvent> eventList;
  Json::Value events = this->client->getLinkEventHistory(modelName, jointName, linkName);
  for(Json::Value::iterator event = events.begin(); event != events.end(); ++event) {
    eventList.push_back(WatcherEvent((*event)[0u].asDouble(), (*event)[1u].asBool(),(*event)[2u].asString()));
  }
  return eventList;
}

//////////////////////////////////////////////////////////
bool TestClient::MonitorLinkEvents(const std::string& modelName, const std::string& jointName,
                                   const std::string& linkName)
{
  return this->client->monitorLinkEvents(modelName, jointName, linkName);
}
