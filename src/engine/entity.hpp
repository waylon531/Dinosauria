//file: entity.hpp
//purpose: entity handler

#pragma once

//STL includes
#include <map>
#include <memory>
#include <iostream>
//custom includes
#include "engine/component.hpp"

namespace engine
{

  class EntityManager;

  struct EntityMask
  {
    int id;
    EntityMask()
    {
      id = 0;
    }
    inline void operator=(const unsigned int ID)
    {
      id=ID;
    }
    inline operator unsigned int()
    {
      return id;
    }
    inline operator unsigned int() const
    {
      return id;
    }
    /** Append to an entity mask
     * @param id id to add to mask
     * @return new mask */
    EntityMask& operator<<(const unsigned int ID)
    {
      id = id | (2 << ID);
      return *this;
    }
    EntityMask& operator+(const unsigned int ID)
    {
      return *this << ID;
    }
  };

  /** An entity to store components */
  class Entity
  {
  public:

    /** The components of this entity */
    std::map<unsigned int, std::shared_ptr<Component>> components;

    /** The numerical ID of this entity */
    unsigned int id;

    /** The string name of this entity */
    std::string name;

    /** The entity manager for this entity */
    EntityManager* entityMgr;

    /** The mask of this entity */
    EntityMask mask;

    /** Initialize this entity
     * @param mgr entity manager to intialize in
     * @param name optional name to use */
    Entity(EntityManager* mgr, const std::string& _name = "");

    Entity()
    {
      id = -1;
    }
    
    /** Add a component to this entity
     * @param component component to add */
    void addComponent(Component* component)
    {
      std::shared_ptr<Component> tmp(component);
      components[component->getID()] = tmp;
      mask << component->getID();
    }

    /** Remove a component based on it's numerical ID
     * @param ID id of component to remove */
    void removeComponent(const unsigned int ID);

    /** Remove a component based on it's name
     * @param name name of component to remove */
    void removeComponent(const std::string& name)
    {
      removeComponent(getComponentID(name));
    }

    /** Check if a component is present in this entity
     * @param ID id of component to check
     * @return if a component with that ID is present in this entity */
    bool hasComponent(const unsigned int ID) const;

    /** Check if a component is present in this entity
     * @param name name of component to check
     * @return if a component with that name is present in this entity */
    bool hasComponent(const std::string& name) const
    {
      return hasComponent(getComponentID(name));
    }

    /** Get a component from this entity based on ID
     * @param ID numerical ID of component
     * @return component with that ID */
    Component* getComponent(const unsigned int ID);

    /** Get a component of a specific type
     * @return component */
    template <typename T> T* getComponent_()
    {
      return reinterpret_cast<T*>(getComponent(getComponentID(T().getName())));
    }

    /** Get a component from this entity based on name
     * @param name name of component
     * @return component with that name */
    Component* getComponent(const std::string& name)
    {
      return getComponent(getComponentID(name));
    }

    /** Check if this entity matches a given mask
     * @param m mask to check
     * @return if m is compatible */
    inline bool matchesMask(const EntityMask m) const
    {
      return (m.id & mask.id) == m.id;
    }
  };
};
