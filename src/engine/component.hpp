/** @file engine/component.hpp */
#pragma once

#include <map>

#include "util.hpp"

namespace engine
{

  extern std::map<std::string, unsigned int> componentIDTable;
  extern int currentComponentID;
  
  /** Get the id for a given name
   * This will create a new entry in the table if the name has not been used before */
  inline unsigned int getComponentID(const std::string& name)
  {
    std::map<std::string, unsigned int>::iterator it = componentIDTable.find(name);
    if(it == componentIDTable.end())
      {
	//add a new component
	componentIDTable[name] = currentComponentID++;
	return currentComponentID-1;
      }
    return it->second;
  }

  /** A chunk of data (abstract, typeless) */
  class BaseProperty
  {
  public:

    /** Destroy */
    ~BaseProperty()
    {
    }

  };

  /** A templated chunk of data */
  template <typename T> class Property: public BaseProperty
  {
  public:

    /** The actual data wrapped */
    T data;

    /** Initialize from this data
     * @param d data */
    Property(const T& d) : data(d)
    {
    }

    /** Destroy */
    ~Property()
    {
    }
  };
  
  /** A chunk of data with an ID */
  class Component
  {
  private:

    /** The data */
    BaseProperty* data;

  public:

    /** The component ID */
    unsigned int ID;

    /** Create a new component
     * @param d data type
     * @param name component type name (this will add a new id if this name has not been used yet) */
    Component(BaseProperty* d, const std::string& name)
    {
      data = d;
      ID = getComponentID(name);
    }

    Component()
    {
      data = NULL;
    }

    /** Destroy */
    ~Component()
    {
    }

    /** Delete the value */
    inline void destroyValue()
    {
      delete data;
    }

    /** Get the data
     * This is a simple conveinance function
     * @param T type of data
     * @return data in native type */
    template <typename T> inline T& getData()
    {
      return ((Property<T>*)data)->data;
    }
  };

};
