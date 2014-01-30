/** @file A single entity in the ECS game model */
#pragma once

#include "util.hpp"
#include "engine/component.hpp"

namespace engine
{
  extern std::map<std::string, unsigned int> entitySigTable;
  extern unsigned int currentEntitySig;

  /** get the entity signature number for a trait (if it does not exist it will be created)
   * @param name name of entity */
  inline unsigned int getEntitySig(const std::string& name)
  {
    std::map<std::string, unsigned int>::iterator it = entitySigTable.find(name);
    if(it == entitySigTable.end())
      {
	//add a new component
	entitySigTable[name] = currentEntitySig++;
	return 1 << (currentEntitySig-1);
      }
    return 1 << it->second;
  }
  
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

    /** The signature of the entity */
    unsigned int sig;

    /** Initialize
     * @param s signature of entity
     * @param name name of the entity */
    Entity(const unsigned int s, const std::string& name="");
    ~Entity();

    /** Add a new component to this entity
     * @param component componenet to add */
    void addComponent(const Component& component);

    /** Get a component by name
     * @param id id of component
     * @return component */
    Component getComponent(const unsigned int id);

    /** Add to this signature
     * @param s signature to add */
    inline void addSig(const unsigned int s)
    {
      sig = sig | s;
    }

    /** Remove from this signature
     * @param s signature to remove */
    inline void removeSig(const unsigned int s)
    {
      sig = sig &  ~s;
    }

    /** Check for a match with a given signature
     * @param s signature to check
     * @return if this entity matches s */
    inline bool checkSig(const unsigned int s)
    {
      return (s & sig) == s;
    }

  };
};
