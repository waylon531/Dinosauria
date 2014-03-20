//file: entity_mgr.hpp
//purpose: handler for an entity IDs and names

#pragma once

//STL includes
#include <map>
#include <string>
//custom includes
#include "engine/entity.hpp"

namespace engine
{

  /** A handler to hold entity Ids and names */
  class EntityManager
  {
  public:

    /** The current next entity ID */
    int nextID = 0;

    /** The map of entities names to IDs */
    std::map<std::string, unsigned int> entityIDMap;

    /** The map of entity IDs to entities */
    std::map<unsigned int, Entity> entities;

    /** Get an entity ID given a name
     * @param name name of entity
     * @return numerical ID */
    unsigned int getEntityID(const std::string& name)
    {
      return entityIDMap[name];
    }

    /** Add a new entity to the entity manager
     * @param name optional entity name
     * @return entity that was created */
    Entity& addEntity(const std::string& name="");

    /** Remove an entity from this manager by ID
     * @param ID entity numerical ID */
    void removeEntity(const unsigned int ID);

    /** Remove an entity from this manager by name
     * @param name entity name */
    void removeEntity(const std::string& name)
    {
      removeEntity(getEntityID(name));
    }

    /** Get an entity from this manager by ID
     * @param ID entity numerical ID
     * @return entity with that ID */
    Entity& getEntity(const unsigned int ID);

    /** Get an entity from this manager by name
     * @param name entity name
     * @return entity with that name */
    Entity& getEntity(const std::string& name)
    {
      return getEntity(getEntityID(name));
    }

    /** Print debug info about contents of entity manager */
    void printDebugInfo();
  };
};
