#include "engine/world.hpp"

//STL includes
#include <algorithm>
//custom includes
#include "engine/system.hpp"

engine::World::World()
{
  mgr = new EntityManager;
}

engine::World::~World()
{
  delete mgr;
  for(int i=0; i<systems.size(); i++)
    {
      delete systems[i];
    }
}

void engine::World::addSystem(System* system)
{
  systems.push_back(system);
  system->parent = this;
}

engine::Entity& engine::World::addEntity(const std::string& name)
{
  Entity& e = mgr->addEntity(name);
  //update system caches
  /*for(std::vector<System*>::iterator it=systems.begin(); it!=systems.end(); it++)
    {
      std::cout << (*it)->mask << std::endl;
      if(e.matchesMask((*it)->mask))
	{
	  (*it)->entities.push_back(e.id);
	}
	}*/
  return e;
}

void engine::World::updateEntity(const unsigned int ID)
{
  Entity& e = mgr->getEntity(ID);
  for(std::vector<System*>::iterator it=systems.begin(); it!=systems.end(); it++)
    {
      if(e.matchesMask((*it)->mask))
	{
	  std::vector<unsigned int>::iterator it2 = std::find((*it)->entities.begin(), (*it)->entities.end(), e.id);
	  if(it2 == (*it)->entities.end())
	  {
#ifdef DEBUG
	    std::cout << "Adding entity " << e.id << " to system with mask " << (*it)->mask << std::endl;
#endif
	      (*it)->entities.push_back(e.id);
	  }
	}
    }
}  

void engine::World::onTick(const float pulse)
{
  for(int i=0; i<systems.size(); i++)
    {
      systems[i]->run(pulse);
    }
  //delete all entities scheduled for deletion
  //TODO: implement entity death
  for(int i=0; i<systems.size(); i++)
    {
      //systems[i]->removeDeadEntities();
    }
  //mgr->removeDeadEntities();
}
