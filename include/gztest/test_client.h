/// 
///  \brief Clientside gztest interface
///  This class is the main interface for the gztest test functionality.
/// 

#ifndef _TEST_CLIENT_H_
#define _TEST_CLIENT_H_

#include "gazebotestclient.h"
#include "Watchers.hh"

namespace gztest
{
class TestClient
{
public:
  TestClient(std::string address);
public:
  ~TestClient();

public:
  /// \brief Test if an object is on a specific surface. Both objects will be determined by the gazebo model instance names
  /// \param[in] _object The name of the object
  /// \param[in] _surface Check, if the object is laying on the specified surface
  bool OnObject(const std::string& object, const std::string& surface);
public:
  /// \brief Resets the world in gazebo to initial state.
  void ResetWorld();
public:
  /// \brief Replace current world with world from given name. Returns true if world was replaced successfully.
  bool LoadWorld(const std::string& world);
public:
  /// \brief Get the position (centroid) of given object
  std::vector<double> GetPosition(const std::string& object);
public:
  /// \brief Get elapsed simtime
  double GetSimtime();
public:
  std::vector<WatcherEvent> GetLinkEventHistory(const std::string& modelName, const std::string& jointName, const std::string& linkName);
public:
  bool MonitorLinkEvents(const std::string& modelName, const std::string& jointName, const std::string& linkName);
private:
  GazeboTestClient* client;
};
}
#endif 
