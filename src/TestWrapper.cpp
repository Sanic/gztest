/*
 * TestWrapper.cpp
 *
 *  Created on: Jul 14, 2014
 *      Author: moritz
 */

#include "TestWrapper.hh"

using namespace gazebo;

// Get access to g_worlds, which is defined in PhysicsIface.cc
extern std::vector<physics::WorldPtr> g_worlds;

namespace gztest
{

//////////////////////////////////////////////////
TestWrapper::TestWrapper() :
    AbstractTestImplementation(new jsonrpc::HttpServer(RPC_PORT))
{
  StartListening();
  this->server = new Server();
  this->gzclientPID = 0;
  this->terminateGZClient = false;
}

/**
 * START ABSTRACT METHOD IMPLEMENTATIONS
 */

//////////////////////////////////////////////////
bool TestWrapper::loadWorld(const std::string& worldPath)
{
  int waitCount = 0;
  int maxWaitCount = 20000;
  gzdbg << "Start waiting\n";
  while (!physics::get_world() && ++waitCount < maxWaitCount)
    common::Time::MSleep(10);
  gzdbg << "Done waiting\n";
  gzmsg << "Reloading world\n";

  sdf::SDFPtr sdf(new sdf::SDF);
  if (!sdf::init(sdf))
  {
    gzerr << "Unable to initialize sdf\n";
    return false;
  }

  if (!sdf::readFile(worldPath, sdf))
  // if (!sdf::readFile("/usr/share/gazebo-1.9/worlds/empty.world", sdf))
  {
    gzerr << "Unable to read SDF file [" << worldPath << "]\n";
    return false;
  }
  sdf::ElementPtr worldElem = sdf->root->GetElement("world");
  if (worldElem)
  {
    physics::WorldPtr new_world = replaceWorld();
    bootWorld(new_world, worldElem);
    return true;
  }
  gzerr << "World SDF Element can't be loaded\n";
  return false;
}

//////////////////////////////////////////////////
physics::WorldPtr TestWrapper::GetWorld()
{
  return this->world;
}

/**
 * START GAZEBO LIFECYCLE CONTROL
 */

//////////////////////////////////////////////////
physics::WorldPtr TestWrapper::replaceWorld()
{
  // Get the currently running world
  physics::WorldPtr old_default_world = physics::get_world();
  // Push an empty world to the g_worlds list
  physics::WorldPtr new_world = physics::create_world("default");
  if (old_default_world)
  {
    gzmsg << "Got an old default world. Stopping world ...\n";
    old_default_world->Stop();
    // Get access to g_worlds, which is defined in PhysicsIface.cc
    g_worlds.erase(g_worlds.begin());
    // 1 World should be left ...
    gzmsg << "Running worlds " << g_worlds.size() << "\n";
  }
  return new_world;
}

//////////////////////////////////////////////////
void TestWrapper::bootWorld(const physics::WorldPtr& new_world, const sdf::ElementPtr& worldElem)
{
  // Create the world
  try
  {
    gzmsg << "Loading world\n";
    physics::load_world(new_world, worldElem);
    gzmsg << "Init world\n";
    physics::init_world(new_world);
    gzmsg << "Run world\n";
    physics::run_world(new_world);
    this->world = new_world;
    killGazeboGUI();
  }
  catch (common::Exception &e)
  {
    gzthrow("Failed to load the World\n" << e);
  }
}

//////////////////////////////////////////////////
void TestWrapper::startGazebo(int _argc, char **_argv)
{
  if (!server->ParseArgs(_argc, _argv))
    return;
  server->Run();
  server->Fini();
  delete server;
}

//////////////////////////////////////////////////
void TestWrapper::startGazeboGUI(char **_argv)
{
  while (!this->terminateGZClient)
  {
    this->gzclientPID = fork();
    int status = 0;
    if (this->gzclientPID == 0)
    {
      execvp("gzclient", _argv);
      return;
    }
    else if (this->gzclientPID == -1)
    {
      gzerr << "Failed to fork to gzclient";
    }
    else
    {
      waitpid(this->gzclientPID, &status, 0);
    }
  }
}

//////////////////////////////////////////////////
void TestWrapper::stopGazeboGUI()
{
  this->terminateGZClient = true;
  killGazeboGUI();
}

//////////////////////////////////////////////////
void TestWrapper::killGazeboGUI()
{
  if (this->gzclientPID)
  {
    kill(this->gzclientPID, SIGKILL);
  }
}

} /* namespace gazebo */

char** vectorToArgv(const std::vector<std::string>& args)
{
  char** _newArgv = (char**)(malloc(sizeof(char*) * args.size()));
  for (int n = 0; n < args.size(); n++)
  {
    _newArgv[n] = (char*)(malloc(sizeof(char) * args[n].size() + 1));
    strcpy(_newArgv[n], args[n].c_str());
  }
  return _newArgv;
}

int main(int _argc, char **_argv)
{
  std::vector<std::string> args;
  bool headless;
  for (int n = 0; n < _argc; n++)
  {
    std::string arg(_argv[n]);
    if (arg == "-hl")
    {
      headless = true;
    }
    else
    {
      args.push_back(_argv[n]);
    }
  }
  gztest::TestWrapper wrapper;
  boost::thread gzclient;
  char** _newArgv = vectorToArgv(args);
  if (!headless)
  {
    gzclient = boost::thread(&gztest::TestWrapper::startGazeboGUI, &wrapper, _newArgv);
  }
  wrapper.startGazebo(args.size(), _newArgv);
  if (!headless)
  {
    wrapper.stopGazeboGUI();
    gzclient.join();
  }

}
