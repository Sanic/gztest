/*
 * TestWrapper.cpp
 *
 *  Created on: Jul 14, 2014
 *      Author: moritz
 */

#include "TestWrapper.hh"

using namespace gazebo;

// Get access to gazebo::g_worlds, which is defined in PhysicsIface.cc
extern std::vector<gazebo::physics::WorldPtr> g_worlds;

namespace gztest
{

//////////////////////////////////////////////////
TestWrapper::TestWrapper() :
		AbstractGazeboTestServer(new jsonrpc::HttpServer(RPC_PORT))
{
	StartListening();
	this->server = new Server();
//	terminate = false;
}

//////////////////////////////////////////////////
bool TestWrapper::onObject(const std::string& object, const std::string& surface)
{
	return OnEntity(this->world->GetModel(object), this->world->GetModel(surface));
}

//////////////////////////////////////////////////
void TestWrapper::resetWorld()
{
	this->world->Reset();
}

//////////////////////////////////////////////////
void TestWrapper::loadWorld(const std::string& worldPath)
{
//	server->Stop();
//	waitForRestart.lock();
//	server->LoadFile(world);
	int waitCount = 0;
	int maxWaitCount = 20000;
	gzdbg << "Start waiting\n";
	while (!gazebo::physics::get_world() && ++waitCount < maxWaitCount)
		gazebo::common::Time::MSleep(10);
	gzdbg << "Done waiting\n";
	gzmsg << "Reloading world\n";

	sdf::SDFPtr sdf(new sdf::SDF);
	if (!sdf::init(sdf))
	{
		gzerr << "Unable to initialize sdf\n";
		return;
	}

	if (!sdf::readFile(worldPath, sdf))
	// if (!sdf::readFile("/usr/share/gazebo-1.9/worlds/empty.world", sdf))
	{
		gzerr << "Unable to read SDF file [" << worldPath << "]\n";
		return;
	}
	sdf::ElementPtr worldElem = sdf->root->GetElement("world");
	if (worldElem)
	{
		// Get the currently running world
		gazebo::physics::WorldPtr old_default_world = gazebo::physics::get_world();
		// Push an empty world to the g_worlds list
		gazebo::physics::WorldPtr new_world = gazebo::physics::create_world("default");
		if (old_default_world)
		{
			msgs::WorldModify worldMsg;
			worldMsg.set_world_name("default");
			worldMsg.set_remove(true);
			this->worldModPub->Publish(worldMsg);
			gzmsg << "Got an old default world. Stopping world ...\n";
			old_default_world->Stop();
			// Get access to gazebo::g_worlds, which is defined in PhysicsIface.cc
			g_worlds.erase(g_worlds.begin());
			// 1 World should be left ...
			gzmsg << "Running worlds " << g_worlds.size() << "\n";
		}
		// Create the world
		try
		{
			gzmsg << "Loading world\n";
			gazebo::physics::load_world(new_world, worldElem);
			gzmsg << "Init world\n";
			gazebo::physics::init_world(new_world);
			gzmsg << "Run world\n";
			gazebo::physics::run_world(new_world);
		} catch (gazebo::common::Exception &e)
		{
			gzthrow("Failed to load the World\n" << e);
		}
	}
	else
	{
		gzerr << "World SDF Element can't be loaded\n";
	}
	gzmsg << "Fin\n";
}

//////////////////////////////////////////////////
bool TestWrapper::OnEntity(physics::EntityPtr entity, physics::EntityPtr onEntity)
{

	// Fail on null
	if (entity == NULL || onEntity == NULL)
	{
		return false;
	}

	math::Box entityBox = entity->GetBoundingBox();
	math::Box onEntityBox = onEntity->GetBoundingBox();
	math::Vector3 center = entityBox.GetCenter();

	//Fast fail if entity is under onEntity
	if (center.z < onEntityBox.GetCenter().z)
	{
		return false;
	}

	return onEntityBox.GetCenter().x + onEntityBox.GetXLength() / 2 >= center.x
			&& onEntityBox.GetCenter().x - onEntityBox.GetXLength() / 2 <= center.x
			&& onEntityBox.GetCenter().y + onEntityBox.GetYLength() / 2 >= center.y
			&& onEntityBox.GetCenter().y - onEntityBox.GetYLength() / 2 <= center.y
			&& center.z - entityBox.GetZLength() / 2 - onEntityBox.GetCenter().z - onEntityBox.GetZLength() / 2
					<= ON_ENTITY_TOLERANCE;
}

void TestWrapper::initGazeboTransport() {
	sleep(2);
	transport::NodePtr node = transport::NodePtr(new transport::Node());
	node->Init("/gztest");
	this->worldModPub = node->Advertise<msgs::WorldModify>("/gazebo/world/modify");
}

//////////////////////////////////////////////////
void TestWrapper::startGazebo(int _argc, char **_argv)
{
//	while (!terminate)
//	{
	if (!server->ParseArgs(_argc, _argv))
		return;
//		waitForRestart.unlock();
	boost::thread(boost::bind(&TestWrapper::initGazeboTransport, this));
	server->Run();
	server->Fini();
	delete server;
//		server = new Server();
//	}
}

} /* namespace gazebo */

int main(int _argc, char **_argv)
{
	gztest::TestWrapper wrapper;
	wrapper.startGazebo(_argc, _argv);
}
