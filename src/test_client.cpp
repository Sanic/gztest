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
bool TestClient::OnObject(const std::string& object, const std::string& surface){
  return this->client->onObject(object,surface);
}

//////////////////////////////////////////////////////////
void TestClient::ResetWorld()
{
	this->client->resetWorld();
}

//////////////////////////////////////////////////////////
void TestClient::LoadWorld(const std::string& world)
{
	this->client->loadWorld(world);
}