/// 
///  \brief Clientside gztest interface
///  This class is the main interface for the gztest test functionality.
/// 

#ifndef _TEST_CLIENT_H_
#define _TEST_CLIENT_H_

#include "gazebotestclient.h"

namespace gztest
{
  class TestClient
  {
    public: TestClient(std::string address);
    public: ~TestClient();

    /// \brief Test if an object is on a specific surface. Both objects will be determined by the gazebo model instance names
    /// \param[in] _object The name of the object
    /// \param[in] _surface Check, if the object is laying on the specified surface
    public: bool OnObject(const std::string& object, const std::string& surface);
    /// \brief Resets the world in gazebo to initial state.
    public: void ResetWorld();
    /// \brief Replace current world with world from given name
    public: void LoadWorld(const std::string& world);
    private: GazeboTestClient* client;
  };
}
#endif 