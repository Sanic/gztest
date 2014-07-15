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
	return this->client->getPosition(object);
}

//////////////////////////////////////////////////////////
double TestClient::GetSimtime()
{
	return this->client->getSimtime();
}
