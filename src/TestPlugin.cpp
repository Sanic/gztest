#include "TestPlugin.hh"

namespace gazebo
{

	//////////////////////////////////////////////////
	TestPlugin::TestPlugin() : AbstractGazeboTestServer(new jsonrpc::HttpServer(RPC_PORT))
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
		this->updateConnection = event::Events::ConnectWorldUpdateBegin(boost::bind(&TestPlugin::OnUpdate, this, _1));
	    StartListening();
	}

	//////////////////////////////////////////////////
	void TestPlugin::OnUpdate(const common::UpdateInfo & /*_info*/)
    {
    	//std::cout << "Box1 on Box2: " << OnEntity("box1", "box2") << std::endl;
    	//boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
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
      return false;// Wont work in a plugin context
    }

	//////////////////////////////////////////////////
    bool TestPlugin::OnEntity(physics::EntityPtr entity, physics::EntityPtr onEntity) {

    	// Fail on null
    	if(entity == NULL || onEntity == NULL) {
    		return false;
    	}

    	math::Box entityBox = entity->GetBoundingBox();
    	math::Box onEntityBox = onEntity->GetBoundingBox();
    	math::Vector3 center = entityBox.GetCenter();

    	//Fast fail if entity is under onEntity
    	if(center.z < onEntityBox.GetCenter().z) {
    		return false;
    	}

    	return 	onEntityBox.GetCenter().x + onEntityBox.GetXLength() / 2 >= center.x &&
    			onEntityBox.GetCenter().x - onEntityBox.GetXLength() / 2 <= center.x &&
    			onEntityBox.GetCenter().y + onEntityBox.GetYLength() / 2 >= center.y &&
    			onEntityBox.GetCenter().y - onEntityBox.GetYLength() / 2 <= center.y &&
    			center.z - entityBox.GetZLength() / 2 - onEntityBox.GetCenter().z - onEntityBox.GetZLength() / 2 <= ON_ENTITY_TOLERANCE;
    }

}
