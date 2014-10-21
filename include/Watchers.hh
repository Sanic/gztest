#include <boost/tuple/tuple.hpp>
#include <boost/functional/hash.hpp>
#include <boost/unordered/unordered_map.hpp>

#ifndef _WATCHERS_HH
#define _WATCHERS_HH

struct Watcher
{

  std::string modelName;
  std::string jointName;
  std::string linkName;
  Watcher(const std::string& modelName, const std::string& jointName, const std::string& linkName)
  {
    this->modelName = modelName;
    this->jointName = jointName;
    this->linkName = linkName;
  }
};

static inline bool operator==(const Watcher &a, const Watcher &b)
{
  return a.modelName == b.modelName && a.jointName == b.jointName && a.linkName == b.linkName;
}

static inline std::size_t hash_value(const Watcher &w)
{
  std::size_t seed = 0;
  boost::hash_combine(seed, w.modelName);
  boost::hash_combine(seed, w.jointName);
  boost::hash_combine(seed, w.linkName);
  return seed;
}

typedef boost::tuple<double, bool, std::string> WatcherEvent;
typedef boost::unordered_map<Watcher, std::vector<WatcherEvent> > WatcherEventMap;
#endif
