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
