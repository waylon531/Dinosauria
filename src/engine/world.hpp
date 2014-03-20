//file: world.hpp
//purpose: ECS game world handling

#pragma once

//STL includes
#include <vector>
//custom includes
#include "engine/entity.hpp"
#include "engine/entity_mgr.hpp"

namespace engine
{
  class EntityManager;
  class System;

  /** A world (contains a entity manager and systems with the ability to execute them) */
  class World
  {
  private:

    /** The systems running in this world */
    std::vector<System*> systems;
    
  public:

    /** The entity manager for this world */
    EntityManager* mgr;

    /** Initialize the world */
    World();

    /** Destroy the world */
    ~World();

    /** Add a new system to this world
     * Note that only entities that were added after this system was added will be added to the cache of this system. In addition, this world will take ownership of that system
     * @param system system to add */
    void addSystem(System* system);

    /** Create a new entity in this world
     * Call this instead of the mgr->addEntity because it updates system caches
     * @param name optional name of entity
     * @return the entity that was created */
    Entity& addEntity(const std::string& name="");

    /** Update the system caches for a particular entity
     * @param ID numeric id of entity */
    void updateEntity(const unsigned int ID);

    /** Update the system caches for a particular entity
     * @param name name of entity */
    void updateEntity(const std::string& name)
    {
      updateEntity(mgr->getEntityID(name));
    }

    /** Execute a cycle of this system
     * @param pulse time since last cycle */
    void onTick(const float pulse);
  };
};
