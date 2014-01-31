/** @file engine/engine.hpp Main game engine implementation */
#pragma once

#include "engine/system.hpp"

namespace engine
{

  /** A manager to handle computations among all entities and systems */
  class EngineManager
  {
  public:

    /** The list of entities */
    std::vector<Entity> entities;

    /** The list of systems */
    std::vector<System*> systems;

    /** Initialize */
    EngineManager();

    /** Destroy */
    ~EngineManager();

    /** Add a new Entity
     * @param entity entity to add */
    inline void addEntity(Entity e)
    {
      entities.push_back(e);
    }

    /** Remove an entity by id
     * @param id id of entity to remove */
    inline void removeEntity(const unsigned int id)
    {
      for(int i=0; i<entities.size(); i++)
	{
	  if(entities[i].id == id)
	    {
	      entities.erase(std::vector<Entity>::iterator(&entities[i]));
	      return;
	    }
	}
      std::cout << "Id " << id << " not found in engine" << std::endl;
      raise(SIGTERM);
    }

    /** Remove an entity by name
     * @param name name of entity to remove */
    inline void removeEntity(const std::string& name)
    {
      for(int i=0; i<entities.size(); i++)
	{
	  if(entities[i].name == name)
	    {
	      entities.erase(std::vector<Entity>::iterator(&entities[i]));
	      return;
	    }
	}
      std::cout << "Name " << name << " not found in engine" << std::endl;
      raise(SIGTERM);
    }

    /** Add a new System
     * @param system system to add */
    inline void addSystem(System* s)
    {
      systems.push_back(s);
    }

    /** Update the engine */
    void update();

  };
};
