#include "engine/entity.hpp"

//STL includes
#include <sstream>
//custom includes
#include "engine/entity_mgr.hpp"

engine::Entity::Entity(EntityManager* mgr, const std::string& _name) : entityMgr(mgr), name(_name)
{
  name = _name;
  entityMgr = mgr;
  components = std::map<unsigned int, std::shared_ptr<Component>>();
  id = entityMgr->nextID++;
  if(name!="")
    {
      mgr->entityIDMap[name] = id;
    }
}

void engine::Entity::removeComponent(const unsigned int ID)
{
  std::map<unsigned int, std::shared_ptr<Component>>::iterator it = components.find(ID);
  if(it == components.end())
    {
      std::stringstream ss;
      ss << "No component found with ID " << ID;
      throw std::runtime_error(ss.str());
    }
  components.erase(it);
}

bool engine::Entity::hasComponent(const unsigned int ID) const
{
  std::map<unsigned int, std::shared_ptr<Component>>::const_iterator it = components.find(ID);
  if(it == components.end())
    {
      return false;
    }
  return true;
}

engine::Component* engine::Entity::getComponent(const unsigned int ID)
{
  std::map<unsigned int, std::shared_ptr<Component>>::iterator it = components.find(ID);
  if(it == components.end())
    {
      std::stringstream ss;
      ss << "No component found with ID " << ID;
      throw std::runtime_error(ss.str());
    }
  return (*it).second.get();
}
