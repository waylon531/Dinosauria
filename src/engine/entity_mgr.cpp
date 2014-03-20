#include "engine/entity_mgr.hpp"

//STL includes
#include <sstream>

engine::Entity& engine::EntityManager::addEntity(const std::string& name)
{
  Entity tmp(this, name);
  entities[tmp.id] = tmp;
  return entities[tmp.id];
}

void engine::EntityManager::removeEntity(const unsigned int ID)
{
  std::map<unsigned int, Entity>::iterator it = entities.find(ID);
  if(it == entities.end())
    {
      std::stringstream ss;
      ss << "No entity found with ID " << ID;
      throw std::runtime_error(ss.str());
    }
  if((*it).second.name != "")
    {
      std::map<std::string, unsigned int>::iterator it2 = entityIDMap.find((*it).second.name);
      if(it2 == entityIDMap.end())
	{
	  std::stringstream ss;
	  ss << "No entity found with name " << (*it).second.name;
	  throw std::runtime_error(ss.str());
	}
      entityIDMap.erase(it2);
    }
  entities.erase(it);
}

engine::Entity& engine::EntityManager::getEntity(const unsigned int ID)
{
  return entities.at(ID);
}

void engine::EntityManager::printDebugInfo()
{
  std::cout << "Contents: " << std::endl;
  for(std::map<unsigned int, Entity>::iterator it=entities.begin(); it!=entities.end(); it++)
    {
      std::cout << (*it).first << "," << (*it).second.id << "," << (*it).second.name << std::endl;
    }
  std::cout << std::endl;
}
