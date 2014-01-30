/** @file A single entity in the ECS game model */
#pragma once

#include "util.hpp"
#include "engine/component.hpp"

namespace engine
{
  /** An entity */
  class Entity
  {
  private:
    
    /** The component list */
    std::vector<Component> components;

  public:

    /** The name of the entity (optional) */
    std::string name;

    /** The ID of the entity */
    unsigned int id;

    /** Initialize
     * @param name name of the entity */
    Entity(const std::string& name="");
    ~Entity();

    /** Add a new component to this entity
     * @param component componenet to add */
    void addComponent(const Component& component);

    /** Get a component by name
     * @param name name of component
     * @return component */
    Component getComponent(const std::string& name);

  };
};
